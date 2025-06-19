
using System.Diagnostics;

using Microsoft.Diagnostics.Tracing.Parsers;
using Microsoft.Diagnostics.Tracing.Session;

class Program
{
    static void Main(string[] args)
    {
        if ((bool)!TraceEventSession.IsElevated())
        {
            return;
        }
        string exePath = @"C:\Users\USER\Desktop\Monitor\file.exe";
        string logPath = @"C:\Users\USER\Desktop\Monitor\detailed_trace_log.txt";
        using var logWriter = new StreamWriter(logPath, append: false);
        using var session = new TraceEventSession("DetailedTraceSession") 
        { StopOnDispose = true };
        session.EnableKernelProvider(KernelTraceEventParser.Keywords.All);
        int targetPid = -1;
        session.Source.Kernel.All += data =>
        {
            if (data.ProcessID != targetPid)
                return;
            string separator = new string('-', 20);
            string header = $"""
            {separator}
             Provider:     {data.ProviderName}
             ProcessID:    {data.ProcessID}
             Event Name:   {data.EventName}
             Task:         {data.TaskName} ({data.Task})
            """;
            string payload = "";
            foreach (var name in data.PayloadNames)
            {
                try
                {
                    var value = data.PayloadByName(name);
                    string valueType = value?.GetType().Name ?? "null";
                    payload += $"    • {name} ({valueType}): {value}\n";
                }
                catch (Exception ex)
                {
                    payload += $"    • {name}: [שגיאה בקריאה: {ex.Message}]\n";
                }
            }
            string fullLog = header + "\n Payload:\n" + payload + separator + "\n";
            Console.WriteLine(fullLog);
            logWriter.WriteLine(fullLog);
            logWriter.Flush();
        };
        Process monitoredProcess = StartProcessAndGet(ref targetPid, exePath);
        monitoredProcess.Exited += (s, e) =>
        {
             Thread.Sleep(1000);  
            session.Dispose(); 
            logWriter.Close(); 
           
        };
        var etwTask = Task.Run(() => session.Source.Process());
        monitoredProcess.WaitForExit();
        Thread.Sleep(2000);
    }
    public static Process StartProcessAndGet(ref int pid, string exePath)
    {
        if (string.IsNullOrWhiteSpace(exePath))
            throw new ArgumentException("נתיב לקובץ אינו תקין.");

        var startInfo = new ProcessStartInfo
        {
            FileName = exePath,
            UseShellExecute = false,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            CreateNoWindow = true
        };
        var process = new Process { StartInfo = startInfo, EnableRaisingEvents = true };
        process.OutputDataReceived += (sender, e) =>
        {
            if (!string.IsNullOrEmpty(e.Data))
                Console.WriteLine($"[STDOUT] {e.Data}");
        };
        process.ErrorDataReceived += (sender, e) =>
        {
            if (!string.IsNullOrEmpty(e.Data))
                Console.WriteLine($"[STDERR] {e.Data}");
        };
        if (!process.Start())
            throw new Exception("התהליך לא הופעל.");
        process.BeginOutputReadLine();
        process.BeginErrorReadLine();
        pid = process.Id;
        return process;
    }
}
