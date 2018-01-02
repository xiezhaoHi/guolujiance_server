using System;
using System.Configuration;
using System.IO;
using System.Text;

namespace RTMC.Core {
	public class Log {
		private static string logPath = Parse.ToString(ConfigurationManager.AppSettings["log"], "app.log");

		public static void WriteMessage() {
			Console.WriteLine();
			WriteLog();
		}
		public static void WriteMessage(string msg) {
			msg = string.Format("{0} {1}", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), msg);
			Console.WriteLine(msg);
			WriteLog(msg);
		}

		public static void WriteLog() {
			StreamWriter sw = new StreamWriter(logPath, true, Encoding.UTF8);
			sw.WriteLine();
			sw.Flush();
			sw.Close();
		}

		public static void WriteLog(string msg) {
			StreamWriter sw = new StreamWriter(logPath, true, Encoding.UTF8);
			sw.WriteLine(msg);
			sw.Flush();
			sw.Close();
		}
	}
}
