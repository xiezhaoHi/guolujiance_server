/// <reference path="/Scripts/libs/jquery-1.10.2.min.js" />
/// <reference path="/Scripts/libs/knockout-3.1.0.js" />

var sys = {
	name: '锅炉在线综合监测信息平台',
	version: '1.0'
};
var url_sys = "/Handler/SystemHandler.ashx";
var url_noauth = "/Handler/NoAuthorityHandler.ashx";
var url_device = "/Handler/DeviceHandler.ashx";
var url_map = "/Handler/MapHandler.ashx";
var url_AlarmRecord = "/Handler/AlarmRecordHandler.ashx";
var url_Config = "/Handler/ConfigHandler.ashx";
var url_devData = "/Handler/DeviceDataHandler.ashx";
var url_up = "/Handler/UpHandler.ashx";
var url_Boiler = "/Handler/BoilerHandler.ashx";

var LeftModel = {
	menus: [],
	perform: function (element, item) {
		if (item == LeftModel.menus[LeftModel.menus.length - 1]) {
			$(".left_title").click(function () {
				if ($(this).next(".left_item").is(":hidden")) {
					$(".left_item:visible").slideUp(300);
					$(this).next(".left_item").slideDown(300);
					$(".shrinkon").removeClass("shrinkon").addClass("shrinkUnder");
					$(this).find(".shrinkUnder").addClass("shrinkon").removeClass("shrinkUnder");
				}
			});
			$(".left_item").height($(".left").height() - 26 * (LeftModel.menus.length + 1) - (LeftModel.menus.length + 1));//计算左边菜单高度
			var left_item = $(".left .left_item ul li a.selected").parents(".left_item");
			//if (left_item.length <= 0) {
			//	left_item = $(".left_item:first");
			//}
			left_item.prev().find(".shrinkUnder").addClass("shrinkon").removeClass("shrinkUnder");
			//left_item.slideDown("slow");
			left_item.show();
		}
	},
	sidebtn: function (sp) {
		var cl = $(sp);
		if (cl.attr('class') === "side_btn2") {
			cl.removeClass("side_btn2");
			cl.addClass("side_btn1");
			$(".left").width(0);
			$(".left .left_allmenu").hide();
			$("#divContent").css('position', 'absolute').css('left', '11px');
		} else {
			cl.removeClass("side_btn1");
			cl.addClass("side_btn2");
			$(".left").width(179);
			$(".left .left_allmenu").show();
			$("#divContent").css('position', 'absolute').css('left', '190px');
		}
		Page.listSize();
	}
};

var TopModel = {
	//注销
	close: function () {
		layer.confirm("确定注销吗？", { icon: 3, title: '提示' }, function () {
			window.location.href = '/Login.aspx';
		});
	},
	//修改口令
	updatePassword: function () {
		$('#txtOldPass').val('');
		$('#txtNewPass').val('');
		$('#txtIsNewPass').val('');
		Page.layer({ id: 'divEditPass', title: '修改密码' });
	},
	savePassword: function () {
		var oldPass = $('#txtOldPass').val();
		var newPass = $('#txtNewPass').val();
		var conPass = $('#txtIsNewPass').val();
		if (oldPass === '') { $('#txtOldPass').focus(); layer.msg('旧密码不能为空！'); return; }
		if (newPass === '') { $('#txtNewPass').focus(); layer.msg('新密码不能为空！'); return; }
		if (newPass != conPass) { $('#txtNewPass').focus(); layer.msg('两次输入密码不一致！<br />请重新输入。'); return; }
		if (newPass === oldPass) { $('#txtNewPass').focus(); layer.msg('新密码和旧密码相同！<br />请重新输入新密码。'); return; }
		JqPost(url_noauth, { 'action': 'ChangePassword', 'oldpass': oldPass, 'newpass': newPass }, function (data) {
			if (data) {
				$("#divEditPass .btncancel").click();
				layer.alert("密码修改成功，请重新登陆！", { icon: 0 }, function () {
					window.location.href = "/Login.aspx?ReturnUrl=" + location.pathname + "&No=1";
				});
			} else {
				$('#txtOldPass').focus(); layer.alert('您输入的旧密码有误！<br />请重新输入。', { icon: 5 });
			}
		});
	},
	UserName: ko.observable('')
};

var BottomModel = {
	name: sys.name,
	version: sys.version
};

// 页面强化
var Page = Page || { permissionsBtns: [] };
(function ($) {
	if (typeof Page.showBtn !== 'function') {
		Page.showBtn = function () {
			if (Page.permissionsBtns.length > 0) {
				for (var i = 0; i < Page.permissionsBtns.length; i++) {
					if (Page.permissionsBtns[i])
						$(".content ." + Page.permissionsBtns[i]).show();
				}
				return;
			}
			JqPost(url_noauth, { action: "getBtnsByUser" }, function (data) {
				Page.permissionsBtns = data;
				for (var i = 0; i < Page.permissionsBtns.length; i++) {
					if (Page.permissionsBtns[i])
						$(".content ." + Page.permissionsBtns[i]).show();
				}
			});
		}
	}

	if (typeof Page.selectAll !== 'function') {
		Page.selectAll = function (names, mainId) {
			var selects1 = document.getElementsByName(names);
			var mains = document.getElementById(mainId);
			if (mains.checked == true) {
				for (var j = 0; j < selects1.length; j++) {
					selects1[j].checked = true;
				}
			} else {
				for (var j = 0; j < selects1.length; j++) {
					selects1[j].checked = false;
				}
			}
		}
	}

	if (typeof Page.layer != 'function') {
		Page.layer = function (para) {
			if (!para) { layer.msg("请传入层Id"); return };
			var uagent = navigator.userAgent;
			var divIndex = layer.open({
				type: 1,
				shift: 5,
				shade: (uagent.indexOf("MSIE 6.0") > 0 || uagent.indexOf("MSIE 7.0") > 0 || uagent.indexOf("MSIE 8.0") > 0) ? 0.2 : 0.01,
				skin: 'layui-layer-shadow',
				title: [para.title != undefined ? para.title : '未命名', 'font-size:14px;font-weight:bold;color:#888888'],
				//area: [para.width != undefined ? para.width : 'auto', para.height != undefined ? para.height : 'auto'], //宽高
				area: para.height ? [(para.width || 'auto'), para.height] : (para.width || 'auto'), //宽高
				content: $('#' + para.id)
			});
			$("#" + para.id + " .btncancel").unbind("click");
			$("#" + para.id + " .btncancel").click(function () {
				layer.close(divIndex);
			});
			return divIndex;
		}
	}

	if (typeof Page.listSize != 'function') {
		Page.listSize = function () {
			var top = 0;
			if ($(".content .con_left:visible").length > 0 || $(".content .con_right:visible").length > 0)
				top += 1;
			else
				top += $(".content .tab").height() + 3
			$(".content .gray_list:visible,.content .main_content:visible").each(function () {
				var con = $(this);
				con.css('top', con.siblings(".panel").height() + con.siblings(".gray_form").height() + top);
			});

			//if ($(".content .main_content:visible").length > 0) {
			//	var con = $(".content .main_content:visible");
			//	con.css('top', con.siblings(".panel").height() + con.siblings(".gray_form").height() + top);
			//}
		}
	}

	if (typeof Page.switchTab != 'function') {
		Page.switchTab = function (tab, div) {
			/// <summary>切换Tab</summary>
			/// <param name="tab" type="String">当前Tab (this)</param>
			/// <param name="div" type="String">需要显示的DIV</param>
			$(".tab_title").removeClass('tab_title').addClass('tab_title1');
			$(tab).removeClass('tab_title1').addClass('tab_title');
			$(".tab_content:visible").hide();
			$("#" + div).show();
			Page.listSize();
		}
	}
	if (typeof Page.Pager != 'function') {
		Page.Pager = function (getPagedData) {
			/// <summary>页面分页模组。</summary>
			/// <param name="getPagedData" type="function">获取分页数据的方法。</param>
			this.Index = ko.observable(1);
			this.Pages = ko.observable(1);
			this.Total = ko.observable(0);
			this.Size = 10;
			this.First = function () {
				if (this.Index() === 1) return;
				getPagedData(1);
			};
			this.Prev = function () {
				var index = parseInt(this.Index()) - 1;
				if (index < 1) {
					return;
				} else {
					this.Index(index);
					getPagedData(index);
				}
			};
			this.Next = function () {
				var index = parseInt(this.Index()) + 1;
				if (index > this.Pages()) {
					return;
				} else {
					this.Index(index);
					getPagedData(index);
				}
			};
			this.Last = function () {
				if (this.Index() === this.Pages()) return;
				getPagedData(this.Pages());
			};
			this.Jump = function () {
				var index = $("#txtGo").val();
				if (index > this.Pages() || index <= 0 || index == this.Index()) {
					return;
				} else {
					getPagedData(index);
				}
			};
			this.GetPagedData = getPagedData;
		};
	}

	if (typeof Page.singleUploadify != 'function') {
		Page.singleUploadify = function (btnId, divFileInfoId, serverPath, action, success) {
			/// <summary>创建上传控件</summary>
			/// <param name="btnId" type="String">上传按钮Id</param>
			/// <param name="divFileInfoId" type="String">上传状态监控容器Id</param>
			/// <param name="serverPath" type="String">服务器上传接口地址</param>
			/// <param name="action" type="String">上传权限名</param>
			/// <param name="success" type="String">上传成功回调函数</param>
			$("#" + btnId).uploadify({
				//开启调试
				'debug': false,
				//是否自动上传
				'auto': true,
				'buttonText': ' ',
				//flash
				'swf': "/Scripts/libs/uploadify/uploadify.swf",
				//文件选择后的容器ID
				'queueID': divFileInfoId,
				'uploader': serverPath,
				'width': '16',
				'height': '16',
				'multi': false,
				'fileTypeDesc': '支持格式：',
				'fileTypeExts': '*.*',
				'fileSizeLimit': '10MB',
				'removeTimeout': 1,
				'removeCompleted': false,
				'formData': { 'action': action || 'UploadData' },
				//'fileObjName':'Filedata',
				//'queueSizeLimit': 0, //允许同时上传的个数 默认值：1 。

				//返回一个错误，选择文件的时候触发
				//'onSelectError': function (file, errorCode, errorMsg) {
				//},
				'onSelect': function (file) {
					$('.tip').show();
				},
				//检测FLASH失败调用
				'onFallback': function () {
					layer.alert("您未安装FLASH控件，无法上传图片！请安装FLASH控件后再试。", { icon: 5 });
				},
				//上传到服务器，服务器返回相应信息到data里
				'onUploadSuccess': function (file, data, response) {
				    $('.tip').hide();
					if (typeof success == 'function') {
						try {
							if (data) data = JSON.parse(data);
						} catch (e) {
							console.log(e.message);
							layer.alert(data, { icon: 5 });
							return;
						}
						success(file, data, response);
					}
				},
				'onUploadError': function (file, errorCode, errorMsg, errorString) {
					if (errorMsg != 'File Cancelled')
						layer.alert(file.name + ' 上传失败： ' + errorMsg, { icon: 5 });
				},
				'onDialogOpen': function () {
					$("#" + divFileInfoId).html('');
				}
				//'onQueueComplete': function (queueData) {
				//}

			});
		}
	}

	//if (typeof Page.GetInfoById != 'function') {
	//	Page.GetInfoById = function (list, id) {
	//		var list2 = ko.mapping.toJS(list);
	//		for (var i = 0; i < list2.length; i++) {
	//			if (list2[i].Id == id) {
	//				return list2[i];
	//			}
	//		}
	//	}
	//}

	if (typeof Page.showTip !== 'function') {
		Page.showTip = function (x, y, color, content, div) {
			/// <summary>指定位置显示标签。</summary>
			/// <param name="x" type="Number">横坐标</param>
			/// <param name="y" type="Number">纵坐标</param>
			/// <param name="color" type="CSS Color">文字和边框颜色</param>
			/// <param name="content" type="String">标签内容</param>
			/// <param name="div" type="Element">标签容器</param>
			div.css({
				position: 'absolute',
				top: y - 5,
				left: x > window.innerWidth - 100 ? x - 100 : x + 10,
				border: '2px solid ' + color,
				padding: '3px 6px',
				'font-size': '11px',
				'border-radius': '5px',
				'background-color': 'black',
				'font-family': 'Verdana, Arial, Helvetica, Tahoma, sans-serif',
				'color': color,
				opacity: 0.8
			});
			div.html(content);
			div.show();
			window.clearAllTimeouts();
			window.setTimeout(function () {
				if (!div.is(':hidden')) { div.hide(); }
			}, 3000);
		};
	}

	if (typeof Page.getDate !== 'function') {
		Page.getDate = function (date) {
			/// <summary>获取时间并格式化。</summary>
			///<param name="date" type="Date">时间</param>
			/// <param name="format" type="String">返回格式yyyy-MM-dd HH:mm:ss</param>
			if (!date)
				date = new Date();
			return date.getFullYear() + "-" + (date.getMonth() + 1) + "-" + date.getDate() + " " + date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
		};
	}

	if (typeof Page.controlVerify !== 'function') {//2015年6月4日  刘其龙添加
		Page.controlVerify = function (cont) {
			var text = $(cont).val();
			var format = $(cont).attr('format');
			if (!text || !format) { return true; }

			var message = "";
			switch (format) {
				case 'zInt'://正整数
					if (!/^[1-9]\d*$/.test(text))
						message = "请输入正整数";
					break;
				case 'fInt'://负整数
					if (!/^-[1-9]\d*$/.test(text))
						message = "请输入负整数";
					break;
				case 'int'://整数
					if (!/^-?[0-9]\d*$/.test(text))
						message = "请输入整数";
					break;
				case 'float'://浮点数
					if (!/^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$/.test(text))
						message = "请输入浮点数";
					break;
				case 'zFloat'://正浮点数
					if (!/^([1-9]\d*\.\d*|0\.\d*[1-9]\d*)$/.test(text))
						message = "请输入正浮点数";
					break;
				case 'fFloat'://负浮点数
					if (!/^-([1-9]\d*\.\d*|0\.\d*[1-9]\d*)$/.test(text))
						message = "请输入负浮点数";
					break;
				case 'num'://数字
					if (!/^-?[1-9]\d*$/.test(text) && !/^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$/.test(text))
						message = "请输入数字";
					break;
				case 'email'://电子邮件
					if (!/^([\w-_]+(?:\.[\w-_]+)*)@((?:[a-z0-9]+(?:-[a-zA-Z0-9]+)*)+\.[a-z]{2,6})$/i.test(text))
						message = "E-mail格式错误";
					break;
				case 'mobile'://移动手机
					if (!/^1[3|4|5|8][0-9]\d{8}$/.test(text))
						message = "手机号码格式错误<br />格式：13800138000";
					break;
				case 'landline'://固定电话
					if (!/^(\d{3,4}\-)?[1-9]\d{6,7}$/.test(text))
						message = "电话号码格式错误<br />格式：028-88888888";
					break;
				case 'phone'://移动手机或固话
					if (!/^(\d{3,4}\-)?[1-9]\d{6,7}$/.test(text) && !/^1[3|4|5|8][0-9]\d{8}$/.test(text))
						message = "电话号码格式错误<br />格式1：028-88888888<br />格式2：13800138000";
					break;
				case 'qq'://qq号码
					if (!/^\d{5,11}$/.test(text))
						message = "QQ号码格式错误";
					break;
				case 'zip'://邮政编码
					if (!/^[1-9]\d{5}(?!\d)/.test(text))
						message = "邮政编码格式错误";
					break;
				case 'idCard'://身份证号码
					if (!/^[1-9]\d{7}((0\d)|(1[0-2]))(([0|1|2]\d)|3[0-1])\d{3}$/.test(text) && !/^[1-9]\d{5}[1-9]\d{3}((0\d)|(1[0-2]))(([0|1|2]\d)|3[0-1])\d{3}([0-9]|X)$/.test(text))
						message = "身份证格式错误";
					break;
				case 'date'://短日期格式
					if (!/^(\d{4})-(0\d{1}|1[0-2])-(0\d{1}|[12]\d{1}|3[01])$/.test(text))
						message = "日期格式错误<br />格式：2015-01-01";
					break;
				case 'time'://长日期格式
					if (!/^(((20[0-3][0-9]-(0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|(20[0-3][0-9]-(0[2469]|11)-(0[1-9]|[12][0-9]|30))) (20|21|22|23|[0-1][0-9]):[0-5][0-9]:[0-5][0-9])$/.test(text))
						message = "日期格式错误<br />格式：2015-01-01 12:00:00";
					break;
				case 'datetime'://长或者短日期格式
					if (!/^(\d{4})-(0\d{1}|1[0-2])-(0\d{1}|[12]\d{1}|3[01])$/.test(text) && !/^(((20[0-3][0-9]-(0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|(20[0-3][0-9]-(0[2469]|11)-(0[1-9]|[12][0-9]|30))) (20|21|22|23|[0-1][0-9]):[0-5][0-9]:[0-5][0-9])$/.test(text))
						message = "日期格式错误<br />格式1：2015-01-01<br />格式2：2015-01-01 12:00:00";
					break;
				default:
					message = "格式验证 “" + format + "”不存在！";
					break;
			}
			if (message != "") {
				layer.tips(message, cont, { tips: [1, '#00B5F0'] });
				$(cont).css('color', 'red').focus();
				return false;
			} else {
				$(cont).css('color', '#666666');
				return true;
			}

		};
	}

	if (typeof Page.Verify !== 'function') {//2015年6月4日  刘其龙添加
		Page.Verify = function (div) {
			if (!div || div == 'undefined') { layer.alert('请为该表单配置Id', { icon: 0 }); return false; }
			var isTrue = true;
			$("#" + div + " .nonEmpty").each(function () {//验证非空字段
				var itext = $(this).parent().next().children();
				if (itext.val() == "") {
					layer.tips('带 * 为必填！', itext, { tips: [1, '#00B5F0'] });
					$(itext).focus();
					isTrue = false;
					return false;
				}
			});
			if (isTrue) {
				$("#" + div + " input[type=text], #" + div + " textarea").each(function () {//验证文本框
					if (!Page.controlVerify(this)) {
						isTrue = false;
						return false;
					}
				});
			}
			return isTrue;
		};
	}
})(jQuery);

// 列表强化
(function ($) {
	if (!ko) return; // 没有knockoutjs环境，则不运行。

	$.rtmc = {};

	function getInstance(clazz, context) {
		var namespaces = clazz.split('.');
		var inst = namespaces.pop();
		for (var i = 0, np; np = namespaces[i]; i++) {
			if (context) context = context[np];
		}
		return context ? context[inst] : null;
	};
	function getFunction(clazz, context /*, args */) {
		var args = [].slice.call(arguments).splice(2);
		var namespaces = clazz.split(".");
		var func = namespaces.pop();
		for (var i = 0; i < namespaces.length; i++) {
			if (context) context = context[namespaces[i]];
		}
		return context ? context[func].apply(this, args) : null;
	};
	function makeList(container) {
		var clazz = $(container).data('list') + '';
		var className = clazz.substr(clazz.lastIndexOf('.') + 1);
		var instance = getInstance(clazz, window);
		var meta = instance.Meta;
		if (!meta) {
			alert(clazz + '.Meta 未定义！');
			return;
		}
		var trTitle = document.createElement('tr');
		var trData = document.createElement('tr');
		if (meta.$chk) {
			// 增加 CheckBox 列
			var chkAll = '<th class="leborno" style="width:30px;"><input type="checkbox" onclick="$(\'.chk_' + className + '\').prop(\'checked\',this.checked);" /></th>';
			var chkRow = '<td class="leborno" style="width:30px;"><input type="checkbox" data-bind="value : ' + meta.$pk + '" class="chk_' + className + '" /></td>';
			$(trTitle).append(chkAll);
			$(trData).append(chkRow);
		}
		var ops = [];
		for (var i = 0, key; key = Object.keys(meta)[i]; i++) {
			if (key.indexOf('_') === 0) { // 只读取下划线开头的字段
				ops.push({ 'key': key, 'val': meta[key] }); // 将操作按钮设置存储起来
			} else if (key.indexOf('$') < 0) {
				var th = document.createElement('th')
				var td = document.createElement('td');
				var val = meta[key];
				th.innerHTML = key;
				if (val.href) {
					var a = document.createElement('a');
					a.setAttribute('target', '_blank');
					a.setAttribute('data-bind', 'attr: { href: ' + val.href + ' }, text: ' + val.bind.replace(/"/g, '\''));
					td.appendChild(a);
				} else {
					td.setAttribute('data-bind', 'text:' + (val.bind ? val.bind.replace(/"/g, '\'') : val.replace(/"/g, '\'')));
				}
				if (val.clazz) {
					th.className = td.className = val.clazz;
					//td.className = val.clazz;
				}
				trTitle.appendChild(th);
				trData.appendChild(td);
			}
		}
		if (ops.length > 0) {
			var th = document.createElement('th');
			var td = document.createElement('td');
			var width = 0; // 操作列宽度
			th.innerHTML = '操作';
			trTitle.appendChild(th);
			for (var j = 0, op; op = ops[j]; j++) {
				var a = document.createElement('a');
				a.innerHTML = op.key.substr(1); // 去掉下划线
				if (op.val.bind) {
					a.setAttribute('data-bind', op.val.bind.replace(/\$pk/g, '\\\'\'+' + meta.$pk + '()+\'\\\''));
				} else {
					a.setAttribute('data-bind', "attr: { 'href': 'javascript:" + op.val.call.replace(/\$pk/g, '\'+' + meta.$pk + '()+\'').replace(/"/g, '\\\'') + "' }");
				}
				a.className = op.val.clazz;
				td.appendChild(a);
				width += a.innerHTML.length * 12 + 22; // 字数 * 字号 + padding值
			}
			th.style.width = (width + 18) + 'px'; // 12px = 为圆角弧左右各5px + 边框1px + 左右内部padding值3px
			trData.appendChild(td);
		}
		var commStart = document.createComment('ko foreach: $data');
		var commEnd = document.createComment('/ko');
		var tbl = document.createElement('table');
		tbl.appendChild(trTitle);
		tbl.appendChild(commStart);
		tbl.appendChild(trData);
		tbl.appendChild(commEnd);
		container.appendChild(tbl);
		container.removeAttribute('data-list');
		//$(container).removeAttr('data-list'); //渲染完成，删除标签
		try {
			ko.applyBindings(instance.Data, container); // 将 RTMC.XXX.Data 绑定到 data-list="RTMC.XXX"
		} catch (e) {
			alert(e.message);
		}
	};
	function makePager(container) {
		var clazz = $(container).data('pager') + '';
		//var className = clazz.substr(clazz.lastIndexOf('.') + 1);
		var pager = getInstance(clazz + '.Pager', window);
		var ul = document.createElement('ul');
		//$(ul).append("<li>每页</li>");
		//$(ul).append('<li><select style="height:20px;" id="cboParent"><option>10</option><option>20</option><option>30</option><option>40</option><option>50</option><option>60</option><option>70</option><option>80</option><option>90</option><option>100</option></select></li>');
		//$(ul).append("<li>条&nbsp;&nbsp;</li>");
		$(ul).append('<li><a href="javascript:' + clazz + '.Pager.First()">首页</a></li>');
		$(ul).append('<li><a href="javascript:' + clazz + '.Pager.Prev()" class="fybtn1" title="上一页">&nbsp;</a></li>');
		$(ul).append('<li data-bind="text: Index() + \'/\' + Pages() + \'页\'"></li>');
		$(ul).append('<li><a href="javascript:' + clazz + '.Pager.Next()" class="fybtn2" title="下一页">&nbsp;</a></li>');
		$(ul).append('<li><a href="javascript:' + clazz + '.Pager.Last()">尾页</a></li>');
		$(ul).append('<li>跳转到第</li><li><input onkeyup="if(this.value.length==1){this.value=this.value.replace(/[^1-9]/g,\'\')}else{this.value=this.value.replace(/\D/g,\'\')}" type="text" id="txtGo" /></li><li>页</li>');
		$(ul).append('<li><a href="javascript:' + clazz + '.Pager.Jump()" class="btn3 fybtn3">转</a></li>')
		container.appendChild(ul);
		container.removeAttribute('data-pager');
		//$(container).removeAttr('data-pager');
		try {
			ko.applyBindings(pager, container); // 将 RTMC.XXX.Pager 绑定到 data-pager="RTMC.XXX"
		} catch (e) {
			alert(e.message);
		}
	};

	$.rtmc.list = function (domspace) {
		/// <summary>生成数据列表结构。</summary>
		/// <param name="domspace" type="Javascript DOM">列表构建范围。</param>
		if (!domspace) domspace = document.body;

		// make list
		var lists = $(domspace).find('[data-list]');
		if (lists.length) {
			for (var i = 0, list; list = lists[i]; i++) {
				makeList(list);
			}
		}

		// make pager
		var pagers = $(domspace).find('[data-pager]');
		if (pagers.length) {
			for (var i = 0, pager; pager = pagers[i]; i++) {
				makePager(pager);
			}
		}

		//ko.applyBindings(RTMC, domspace);
	};

	function makeForm(container) {
		var clazz = $(container).data('form') + '';
		var className = clazz.substr(clazz.lastIndexOf('.') + 1);
		var instance = getInstance(clazz, window);
		if (!instance) {
			container.innerHTML = clazz + '未定义！';
			return;
		}
		var form = instance.Form;
		if (!form) return;
		var tbl = document.createElement('table');
		var div = document.createElement('div'); // 操作按钮div
		div.className = 'btns';
		var endRow = 0;
		var keys = Object.keys(form.cells); // 键集合    	
		for (var i = 0; i < Math.ceil(keys.length / form.column) ; i++) {
			var tr = document.createElement('tr');
			for (var j = 0; j < form.column; j++) {
				var key = keys[i * form.column + j];
				if (!key) {
					endRow = endRow || (j === 0 ? i - 1 : i);
					if (endRow === i) {
						var tdKey = document.createElement('td');
						tdKey.className = form.keyClass;
						tr.appendChild(tdKey);
						var tdVal = document.createElement('td');
						tdVal.className = form.valClass;
						tr.appendChild(tdVal);
						continue;
					} else {
						break;
					}
				}
				var val = form.cells[key];
				if (key.indexOf('_') === 0) {
					endRow = endRow || (j === 0 ? i - 1 : i);
					var a = document.createElement('a');
					a.innerHTML = key.substr(1);
					a.className = val.clazz;
					if (val.verify) {
						a.setAttribute('href', 'javascript:if (Page.Verify(\'' + $(container).attr("id") + '\')){' + val.call + '};');
					} else {
						a.setAttribute('href', 'javascript:' + val.call);
					}

					div.appendChild(a);
					//div.innerHTML += '&nbsp;&nbsp;&nbsp;&nbsp;';
					if (endRow === i) {
						var tdKey = document.createElement('td');
						tdKey.className = form.keyClass;
						tr.appendChild(tdKey);
						var tdVal = document.createElement('td');
						tdVal.className = form.valClass;
						tr.appendChild(tdVal);
					}
					continue;
				}
				var tdKey = document.createElement('td');
				tdKey.className = form.keyClass;
				var keyval = key.replace(/\$\d/g, '');
				tdKey.innerHTML = keyval === '$blank' ? '' : keyval.replace('*', '<font class="nonEmpty">*</font>') + '：';
				tr.appendChild(tdKey); // 添加键td
				var tdVal = document.createElement('td');
				tdVal.className = form.valClass;
				//tdVal.appendChild(controlFactory(val, instance)); // 使用控件工厂制作控件
				$(tdVal).append(controlFactory(val, instance));
				if (val.ext) tdVal.innerHTML += val.ext;
				if (val.colspan && val.colspan > 1 && val.colspan + j <= form.column) { // 处理 colspan
					for (var k = 0; k < val.colspan - 1; k++) {
						keys.splice(i * form.column + j, 0, '');
					}
					j = j + val.colspan - 1;
					tdVal.setAttribute('colspan', val.colspan * 2 - 1 + '');
				}
				tr.appendChild(tdVal);
			}
			if (!endRow || i <= endRow) tbl.appendChild(tr);
		}
		container.appendChild(tbl);
		if (div.innerHTML) container.appendChild(div);
		container.removeAttribute('data-form');
		try {
			ko.applyBindings(instance.Entity, container); // 将 RTMC.XXX.Entity 绑定到 data-form="RTMC.XXX"
		} catch (e) {
			alert(e.message);
		}
	};
	function controlText(cell, instance) {
		/// <summary>创建文本框。</summary>
		var cont = document.createElement('input');
		cont.setAttribute('type', 'text');
		cont.setAttribute('data-bind', 'value:' + cell.bind);
		cont.setAttribute('id', 'txt' + cell.bind);
		cont.className = cell.clazz;
		cont.maxLength = cell.max || 255;
		if (cell.format != undefined) {//2015年6月4日  刘其龙添加
			cont.setAttribute('format', cell.format);
			cont.setAttribute('onblur', 'Page.controlVerify(this)');
		}
		return cont;
	};
	function controlPassword(cell, instance) {
		/// <summary>创建文本框。</summary>
		var cont = document.createElement('input');
		cont.setAttribute('type', 'password');
		cont.setAttribute('data-bind', 'value:' + cell.bind);
		cont.setAttribute('id', 'pwd' + cell.bind);
		cont.className = cell.clazz;
		cont.maxLength = cell.max || 255;
		if (cell.format != undefined) {//2015年6月4日  刘其龙添加
			cont.setAttribute('format', cell.format);
			cont.setAttribute('onblur', 'Page.controlVerify(this)');
		}
		return cont;
	};
	function controlSelect(cell, instance) {
		var cont = document.createElement('select');
		cont.className = cell.clazz;
		cont.setAttribute('id', 'sel' + cell.bind);
		if (cell.onchange) cont.setAttribute('onchange', cell.onchange); // 绑定 onchange 事件
		var pool = instance.Entity['Available' + cell.bind] = ko.mapping.fromJS([]);
		if (!cell.title) cell.title = 'Key';
		if (!cell.value) cell.value = 'Value';
		cont.setAttribute('data-bind', 'options: Available' + cell.bind + ', optionsText: \'' + cell.title + '\', optionsValue: \'' + cell.value + '\', value : ' + cell.bind);
		if (cell.options) {
			for (var i = 0, op; op = cell.options[i]; i++) {
				var o = {};
				o[cell.title] = op[0];
				o[cell.value] = op[1];
				pool.push(o);
			}
		}
		return cont;
	};
	function controlTextArea(cell, instance) {
		var cont = document.createElement('textarea');
		cont.className = cell.clazz;
		cont.setAttribute('data-bind', 'value:' + cell.bind);
		cont.setAttribute('id', 'txt' + cell.bind);
		cont.rows = cell.rows || 1;
		if (cell.format) {//2015年6月4日  刘其龙添加
			cont.setAttribute('format', cell.format);
			cont.setAttribute('onblur', 'Page.controlVerify(this)');
		}
		return cont;
	};
	function controlDatePicker(cell, instance) {
		var cont = document.createElement('input');
		cont.setAttribute('type', 'text');
		if (cell.readOnly) cont.setAttribute('readOnly', cell.readOnly);
		cont.setAttribute('data-bind', 'value:' + cell.bind);
		cont.setAttribute('onclick', 'WdatePicker({ lang: \'zh-cn\', dateFmt: \'' + (cell.format ? cell.format : 'yyyy-MM-dd') + '\' })');
		cont.setAttribute('onblur', '$(this).change();'); // 用于解决 My97DatePicker 为文本框赋值时，knockoutjs 模型无法获取属性更新的问题
		cont.setAttribute('id', 'dat' + cell.bind);
		cont.className = cell.clazz + " Wdate";
		cont.maxLength = 20;
		if (cell.format) {//2015年6月4日  刘其龙添加
			cont.setAttribute('format', cell.format);
			cont.setAttribute('onblur', 'Page.controlVerify(this)');
		}
		return cont;
	};
	function controlBlank() {
		var cont = document.createElement('span');
		cont.innerHTML = "";
		return cont;
	};
	function controlFile(cell, instance) {
		var file = document.createElement('input');
		var div = document.createElement('div');
		var btn = document.createElement('input');
		var cont = document.createElement('span');

		file.setAttribute('type', 'text');
		file.setAttribute('data-bind', 'value:' + cell.bind);
		file.setAttribute('id', 'fil' + cell.bind);
		file.style.cssFloat = 'left';
		file.className = cell.clazz;
		file.maxLength = 255;
		div.setAttribute('id', 'divQueue');
		div.className = "tip hide";
		document.body.appendChild(div);
		btn.setAttribute('id', 'btn' + cell.bind);
		cont.appendChild(file);
		cont.appendChild(btn);
		$(document).ready(function () {
			Page.singleUploadify('btn' + cell.bind, 'divQueue', cell.serverUrl, cell.action, cell.callback || function (upfile, data, response) {
				if (data) {
					file.value = cell.format ? data[cell.format] : data;
					$(file).change();
				}
			});
		});
		return cont;
	};
	function controlTreePicker(cell, instance) {
		/// <summary>创建一个查询框。额外关键字：title/弹出框标题，width/弹出框宽度（默认为400px），url/查询树的接口地址，action/查询树的请求行为名</summary>
		var pool = instance.Entity['Available' + cell.bind] = ko.mapping.fromJS([]);
		var cont = document.createElement('span');
		var pkr = document.createElement('input');
		var txt = document.createElement('input');
		var img = document.createElement('img');
		var rst = document.createElement('img');
		var div = document.createElement('div');
		var ul = document.createElement('ul');

		pkr.setAttribute('type', 'hidden');
		pkr.setAttribute('id', 'pkr' + cell.bind);
		pkr.setAttribute('data-bind', 'value:' + cell.bind);

		txt.setAttribute('type', 'text');
		txt.setAttribute('id', 'txt' + cell.bind);
		txt.setAttribute('readOnly', true);
		txt.className = cell.clazz;
		txt.onkeydown = function (e) { return false; }; // 屏蔽IE、Chrome浏览器的退格键行为。
		txt.onkeypress = function (e) { return false; }; // 屏蔽Firefox、Opera浏览器的退格键行为。

		img.className = 'choice';
		img.title = '单击选择。';
		img.src = '/Images/icons/search2.png';
		img.onclick = function (e) { Page.layer({ id: 'tree' + cell.bind, title: cell.title, width: cell.width || '400px' }); };

		rst.className = 'reset';
		rst.title = '单击清空。';
		rst.src = '/Images/icons/reset2.png';
		rst.onclick = function (e) { pkr.value = ''; txt.value = ''; };

		div.id = 'tree' + cell.bind;
		div.className = 'layer';
		div.style.maxHeight = '400px';

		ul.id = 'ulTree' + cell.bind;
		ul.className = 'ztree';

		$(function () {
			if (cell.data) {
				ko.mapping.fromJS(cell.data, pool);
				$.fn.zTree.init($('#' + ul.id), {
					data: { simpleData: { enable: true } },
					callback: {
						onClick: function (event, treeId, treeNode, clickFlag) {
							pkr.value = treeNode.id;
							$(pkr).change(); // 触发 ko 记录绑定属性的变化。
							txt.value = treeNode.name;
							layer.close(layer.index);
						}
					}
				}, cell.data);
			} else {
				if (!cell.url) throw cell.bind + ' 中缺少 url 参数。';
				if (!cell.action) throw cell.bind + ' 中缺少 action 参数。';
				$.fn.zTree.init($('#' + ul.id), {
					async: {
						enable: true,
						url: cell.url,
						autoParam: ['id=pid'],
						otherParam: { 'action': cell.action },
						dataFilter: cell.filter || function (treeId, parentNode, childNodes) {
							for (var i = 0, node; node = childNodes[i]; i++) {
								pool.push(node);
							}
							return childNodes;
						}
					},
					callback: {
						onClick: function (event, treeId, treeNode, clickFlag) {
							pkr.value = treeNode.id;
							$(pkr).change(); // 触发 ko 记录绑定属性的变化。
							txt.value = treeNode.name;
							layer.close(layer.index);
						}
					}
				});
			}
		});

		div.appendChild(ul);
		document.body.appendChild(div);
		cont.appendChild(pkr);
		cont.appendChild(txt);
		cont.appendChild(img);
		cont.appendChild(rst);
		return cont;
	};
	function controlDefault() {
		var cont = document.createElement('span');
		cont.innerHTML = cell.bind + ' 中未定义的控件类型：' + cell.type;
		return cont;
	};
	function controlFactory(cell, instance) {
		switch (cell.type) {
			case 'text': return controlText(cell, instance);
			case 'password': return controlPassword(cell, instance);
			case 'select': return controlSelect(cell, instance);
			case 'textarea': return controlTextArea(cell, instance);
			case "date": return controlDatePicker(cell, instance);
			case "blank": return controlBlank();
			case "file": return controlFile(cell, instance);
			case "tree": return controlTreePicker(cell, instance);
			default: return controlDefault();
		}
	};
	$.rtmc.form = function (domspace) {
		if (!domspace) domspace = document.body;

		var forms = $(domspace).find('[data-form]');
		if (forms.length) {
			for (var i = 0, form; form = forms[i]; i++) {
				makeForm(form);
			}
		}
	};
})(jQuery);

// 初始化脚本
$(function () {
	JqPost(url_noauth, { action: "getMenusByUser" }, function (data) {
		if (data.userName) { TopModel.UserName(data.userName); }
		if (data.menus === null || data.menus === []) {
			window.location.href = "/";
		} else {
			LeftModel.menus = data.menus;
			ko.applyBindings(LeftModel, divLeft);
		}
	});
	ko.applyBindings(TopModel, divTop);
	ko.applyBindings(BottomModel, divBottom);
	Page.listSize();

	$(window).resize(function () {
		//var tab = $(".tab_content:visible");
		//if (tab.length == 1) {
		//	//tab.remove(tab.find('.hide'));
		//	Page.listSize(tab.attr('id'));
		//} else {
		//}
		Page.listSize();
		$(".left_item").height($(".left").height() - 26 * (LeftModel.menus.length + 1) - (LeftModel.menus.length + 1)); //计算左边菜单高度
	});

	document.title = sys.name + ' - ' + document.title;
});
