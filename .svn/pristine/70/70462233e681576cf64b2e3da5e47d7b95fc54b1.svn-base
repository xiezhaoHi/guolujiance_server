using RTMC.BLL;
using RTMC.Source;
using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Objects;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Media;
using System.Windows.Forms;
using System.Net.WebSockets;
using System.Net.Sockets;
using System.Net;
using TestingServices;

namespace RTMC.TestingServices {
	public partial class Service1 : ServiceBase {
		public Service1() {
			InitializeComponent();
		}

		private System.Threading.Timer timer;
		protected override void OnStart(string[] args) {
			this.timer = new System.Threading.Timer(new TimerCallback(timer1_Tick), this, 0, 10000);
		}



		//public void OnStop() {
		//	bool b = GetDataManager.GetVirData();
		//	if (b) {
		//		SoundPlayer sp = new SoundPlayer(Application.StartupPath.Substring(0, Application.StartupPath.IndexOf("bin")) + @"报警声音.wav");
		//		sp.PlayLooping();
		//	}
		//}

		private void timer1_Tick(object state) {
			bool b = GetDataManager.GetVirData();
		}
	}
}
