<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="RTMC.Web.Login" %>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title>锅炉在线综合监测信息平台</title>
	<style type="text/css">
		html, body { width: 100%; height: 100%; padding: 0px; margin: 0px; background: url(/Images/login/login_bg123r.jpg) repeat-x; overflow: hidden; }

		.warp { position: relative; width: 1214px; height: 300px; margin: 150px auto 20px; background: url(/Images/login/bg1.png) no-repeat center; z-index: 999; }

			.warp .content { position: absolute; width: 385px; height: 410px; right: 120px; bottom: -20px; border-collapse: collapse; border-spacing: 0px; display: table; background: url(/Images/login/loginbg.png) no-repeat center; }

			.warp .titl { position: absolute; width: 510px; height: 84px; left: 0px; top: -55px; border-collapse: collapse; border-spacing: 0px; background: url(/Images/login/titlebg.png) no-repeat center; }

			.warp .content td { height: 32px; padding: 0px; font-size: 14px; }
		/*.bg8r { position: absolute; bottom: 0px; width: 100%; height:290px; min-width:980px; background: url(/Images/login/login_bg8r.jpg) repeat-x; }*/
		/*.bg8r .bg7 { position:absolute; bottom:0px; left:0px; width:700px; height:290px; background:url(/Images/login/login_bg7.jpg) no-repeat;}*/
		/*.bg8r .bg9 { position:absolute; bottom:0px; right:0px; width:280px; height:290px; background:url(/Images/login/login_bg9.jpg) no-repeat;}*/
		.txt { width: 256px; height: 43px; line-height: 43px; font-size: 20px; background-color: transparent; border: 0px; padding-left: 50px; }

		.btn { width: 75px; height: 23px; border: 0px; cursor: pointer; }

		.layui-layer, .layui-layer-btn0, .layui-layer-btn1 { border-radius: 5px; -moz-border-radius: 5px; -webkit-border-radius: 5px; }
		.layui-layer-title { border-radius: 5px 5px 0 0; -moz-border-radius: 5px 5px 0 0; -webkit-border-radius: 5px 5px 0 0; }
	</style>
	<!--[if IE 6]>
	<script type="text/javascript" src="Scripts/EvPNG.js"></script>
	<script type="text/javascript">EvPNG.fix('div, li, a, img, dt');</script>
	<![endif]-->
	<script src="/Scripts/libs/jquery-1.10.2.min.js"></script>
	<script src="/Scripts/utils/rtmc-util.js"></script>
	<script type="text/javascript" src="/Scripts/libs/layer-v1.9.3/layer/layer.js"></script>
	<script type="text/javascript">
		function dologin() {
			if ($("#txtUserName").val() == "") {
				layer.alert("请输入用户名！", { icon: 5 }, function (i) {
					$("#txtUserName").focus();
					layer.close(i);
				});
				return false;
			} else if ($("#txtPassword").val() == "") {
				layer.alert("请输入密码！", { icon: 5 }, function (i) {
					$("#txtPassword").focus();
					layer.close(i);
				});
				return false;
			} else if ($("#txtValidCode").val() == "") {
				layer.alert("请输入验证码！", { icon: 5 }, function (i) {
					$("#txtValidCode").focus();
					layer.close(i);
				});
				return false;
			}
			return true;
		}

		function Login() {
			if (dologin()) {
				JqPost('/Handler/NoAuthorityHandler.ashx', { action: "login", name: $("#txtUserName").val(), pwd: $("#txtPassword").val(), code: $("#txtValidCode").val() }, function (data) {
					if (window.request && window.request.ReturnUrl) {
						window.location.href = window.request.ReturnUrl;
					} else
						window.location.href = "/Index.aspx";
				}, function (e) {
					layer.alert(e.responseText, { icon: 5 }, function (i) {
						if (e.responseText == "验证码错误！") {
							$("#imgCode").click();
							$("#txtValidCode").val('').focus();
						} else {
							$("#txtUserName").focus();
						}
						layer.close(i);
					});
				});
			}
		}
		$(document).ready(function () {
			$("#txtUserName,#txtPassword,#txtValidCode").keyup(function (e) {
				// 回车键事件  
				if (e.which == 13) {
					Login();
				}
			});
			$("#txtUserName").focus();
			if (window.request && window.request.No != 1) {
				layer.alert("登陆超时，请重新登陆！", { icon: 0 });
			}
		});
	</script>
</head>
<body>

	<div class="warp">
		<div class="titl"></div>
		<div class="content">
			<div style="margin-top: 122px; margin-left: 42px;">
				<input placeholder="请输入用户名" value="admin" id="txtUserName" name="UserName" type="text" class="txt" maxlength="16" tabindex="1" />
			</div>
			<div style="margin-top: 17px; margin-left: 42px;">
				<input style="" placeholder="请输入密码" value="123456" id="txtPassword" name="Password" type="password" class="txt" maxlength="16" tabindex="2" />
			</div>
			<div style="margin-top: 15px; margin-left: 42px; overflow: hidden">
				<input placeholder="输入验证码" id="txtValidCode" name="ValidCode" type="text" class="txt" style="width: 116px; float: left;" maxlength="4" tabindex="3" onkeyup="this.value=this.value.toUpperCase();" />
				<img id="imgCode" src="ValidCode.aspx" style="width: 120px; height: 43px; border: 1px solid #999; float: left; margin-left: 10px; border: none" title="点击刷新验证码！" alt="验证码，点击刷新！" onclick="this.src='ValidCode.aspx?rand='+Math.random()" />
			</div>
			<div style="margin-top: 22px; margin-left: 42px;">
				<a href="javascript:Login();" style="display: block; width: 305px; height: 60px; border: none; cursor: pointer" tabindex="4"></a>
			</div>
		</div>
	</div>
	<div style="position:fixed; bottom:10px; width:100%; text-align: center; line-height:1.5; font-size:smaller; font-family:'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif;">©2014-2015 <a href="http://www.cdraintime.com">www.cdraintime.com</a> 保留所有权利</div>
</body>
</html>
