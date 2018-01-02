using RTMC.BLL;
using RTMC.Permissions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;

namespace RTMC.Web {
	public class Global : System.Web.HttpApplication {

		protected void Application_Start(object sender, EventArgs e) {
			//MapManager.GetDeviceDataByIdFirst(new Guid("c663016d-8f08-44a8-bff6-909b0abd9759"));
			PermissionLibrary.Initial();
		}

		protected void Session_Start(object sender, EventArgs e) {

		}

		protected void Application_BeginRequest(object sender, EventArgs e) {

		}

		protected void Application_AuthenticateRequest(object sender, EventArgs e) {

		}

		protected void Application_Error(object sender, EventArgs e) {

		}

		protected void Session_End(object sender, EventArgs e) {

		}

		protected void Application_End(object sender, EventArgs e) {

		}
	}
}