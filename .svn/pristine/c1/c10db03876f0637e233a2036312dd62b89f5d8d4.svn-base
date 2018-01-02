using RTMC.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace RTMC.Web {
    public partial class ValidCode : Page {
        protected void Page_Load(object sender, EventArgs e) {
            string code = Function.GetRandomString(4);
            Session["ValidCode"] = code;
            Response.ContentType = "image/png";
            Response.BinaryWrite(Function.GetValidImage(code).ToArray());
        }
    }
}