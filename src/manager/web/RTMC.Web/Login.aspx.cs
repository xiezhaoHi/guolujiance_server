using RTMC.BLL;
using RTMC.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace RTMC.Web {
    public partial class Login : Page {

        protected string Message { get; set; }
        protected void Page_Load(object sender, EventArgs e) {
            Session.Abandon();
            FormsAuthentication.SignOut();
        }
    }
}