<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Index.aspx.cs" Inherits="RTMC.Web.Index" %>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title>锅炉综合检测信息化平台</title>
	<link type="text/css" href="/CSS/global.css" rel="stylesheet" />
	<link href="CSS/zTreeStyle/zTreeStyle.css" rel="stylesheet" />
	<!--[if ie 6]>
	<script type="text/javascript" src="/Scripts/libs/EvPNG.js"></script>
	<script type="text/javascript">EvPNG.fix('div, li, a, img, dt');</script>
	<![endif]-->
	<script type="text/javascript" src="/Scripts/libs/jquery-1.10.2.min.js"></script>
	<script type="text/javascript" src="/Scripts/libs/knockout-3.1.0.js"></script>
	<script type="text/javascript" src="/Scripts/libs/knockout.jquery.trafficCop.js"></script>
	<script type="text/javascript" src="/Scripts/libs/knockout.jquery.infuser.js"></script>
	<script type="text/javascript" src="/Scripts/libs/knockout.jquery.templateEngine.js"></script>
	<script type="text/javascript" src="/Scripts/libs/knockout.mapping-latest.js"></script>
	<script type="text/javascript" src="/Scripts/libs/layer-v1.9.3/layer/layer.js"></script>
	<script type="text/javascript" src="/Scripts/libs/moment-with-locales.min.js"></script>
	<script type="text/javascript" src="/Scripts/libs/DatePicker/WdatePicker.js" defer="defer"></script>
	<script type="text/javascript" src="/Scripts/libs/zTree/jquery.ztree.core-3.5.min.js"></script>
	<script type="text/javascript" src="/Scripts/libs/uploadify/jquery.uploadify.min.js"></script>
	<script type="text/javascript" src="/Scripts/libs/chosen/chosen.jquery.min.js"></script>
	<script type="text/javascript" src="/Scripts/utils/rtmc-util.js"></script>
	<script type="text/javascript" src="/Scripts/project/config.js"></script>
	<script type="text/javascript" src="/Scripts/project/poco.js"></script>
	<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=3479b9452a6f16cf902bad1e31e96422"></script>
	<style>
		.content .panel .btn1, .content .panel .btn2, .content .panel .btn3, .content .gray_list .btn1, .content .gray_list .btn2, .content .gray_list .btn3 { display: inline; }
		#map { width: 100%; height: 550px; }
	</style>
	<script type="text/javascript">
		var zNodes = [
				{ id: 1, pId: 0, name: "父节点1 - 展开", open: true },
				{ id: 11, pId: 1, name: "父节点11 - 折叠" },
				{ id: 111, pId: 11, name: "叶子节点111" },
				{ id: 112, pId: 11, name: "叶子节点112" },
				{ id: 113, pId: 11, name: "叶子节点113" },
				{ id: 114, pId: 11, name: "叶子节点114" },
				{ id: 12, pId: 1, name: "父节点12 - 折叠" },
				{ id: 121, pId: 12, name: "叶子节点121" },
				{ id: 122, pId: 12, name: "叶子节点122" },
				{ id: 123, pId: 12, name: "叶子节点123" },
				{ id: 124, pId: 12, name: "叶子节点124" },
				{ id: 13, pId: 1, name: "父节点13 - 没有子节点", isParent: true },
				{ id: 2, pId: 0, name: "父节点2 - 折叠" },
				{ id: 21, pId: 2, name: "父节点21 - 展开", open: true },
				{ id: 211, pId: 21, name: "叶子节点211" },
				{ id: 212, pId: 21, name: "叶子节点212" },
				{ id: 213, pId: 21, name: "叶子节点213" },
				{ id: 214, pId: 21, name: "叶子节点214" },
				{ id: 22, pId: 2, name: "父节点22 - 折叠" },
				{ id: 221, pId: 22, name: "叶子节点221" },
				{ id: 222, pId: 22, name: "叶子节点222" },
				{ id: 223, pId: 22, name: "叶子节点223" },
				{ id: 224, pId: 22, name: "叶子节点224" },
				{ id: 23, pId: 2, name: "父节点23 - 折叠" },
				{ id: 231, pId: 23, name: "叶子节点231" },
				{ id: 232, pId: 23, name: "叶子节点232" },
				{ id: 233, pId: 23, name: "叶子节点233" },
				{ id: 234, pId: 23, name: "叶子节点234" },
				{ id: 3, pId: 0, name: "父节点3 - 没有子节点", isParent: true }
		];
		var data = [
			{ PrimaryKey: 1, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 2, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 3, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 4, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 5, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 6, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: null },
			{ PrimaryKey: 7, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 8, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 9, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 10, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 11, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 12, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 13, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 14, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) },
			{ PrimaryKey: 15, PropertyA: '马云', PropertyB: 38, PropertyC: 1, PropertyD: new Date(1987, 11, 5) }
		];
		var model = function () {
			this.PrimaryKey = null;
			this.PropertyA = null;
			this.PropertyB = null;
			this.PropertyC = null;
			this.PropertyD = null;
			this.PropertyE = null;
			this.EntityState = RTMC.EntityState.UNCHANGED;
		};
		var idx = {};

		idx.example = {
			Model: model,
			Data: ko.mapping.fromJS([]),
			Meta: {
				$pk: 'PrimaryKey',
				$chk: true,
				姓名: 'PropertyA',
				年龄: 'PropertyB',
				性别: 'PropertyC()?"男":"女"',
				生日: 'PropertyD()? moment(PropertyD()).format("YYYY-MM-DD"):"-"'
			},
			Pager: new Page.Pager(function (index) {
				var me = idx.example.Pager;
				var json = { 'data': data.slice((index - 1) * me.Size, index * me.Size - 1), 'total': data.length };
				if (json) {
					ko.mapping.fromJS(json.data, idx.example.Data);
					me.Total(json.total);
					me.Pages(Math.ceil(json.total / me.Size));
					me.Index(index);
				}
				Page.showBtn();
			}),
			Action: null,
			Entity: ko.mapping.fromJS(new model()),
			Form: {
				column: 2,
				keyClass: 'w15 tdr',
				valClass: 'w35',
				cells: {
					'*姓名': { clazz: 'w80', type: 'text', bind: 'PropertyA' },
					年龄: { clazz: 'w80', type: 'text', bind: 'PropertyB', format: 'zInt' },//format  验证的格式
					性别: { clazz: 'w85', type: 'select', bind: 'PropertyC', options: [['-请选择-', ''], ['男', 1], ['女', 0]] },
					生日: { clazz: 'w80', type: 'date', bind: 'PropertyD' },
					部门: { clazz: 'w80', type: 'tree', bind: 'PropertyE', title: '选择部门', width: '300px', data: zNodes },
					整数: { clazz: 'w80', type: 'text', format: 'int' },
					负整数: { clazz: 'w80', type: 'text', format: 'fInt' },
					浮点数: { clazz: 'w80', type: 'text', format: 'float' },
					负浮点数: { clazz: 'w80', type: 'text', format: 'fFloat' },
					正浮点数: { clazz: 'w80', type: 'text', format: 'zFloat' },
					数字: { clazz: 'w80', type: 'text', format: 'num' },
					Email: { clazz: 'w80', type: 'text', format: 'email' },
					手机: { clazz: 'w80', type: 'text', format: 'mobile' },
					固话: { clazz: 'w80', type: 'text', format: 'landline' },
					手机或固话: { clazz: 'w80', type: 'text', format: 'phone' },
					QQ: { clazz: 'w80', type: 'text', format: 'qq' },
					邮政编码: { clazz: 'w80', type: 'text', format: 'zip' },
					身份证: { clazz: 'w80', type: 'text', format: 'idCard' },
					短日期: { clazz: 'w80', type: 'text', format: 'date' },
					长日期: { clazz: 'w80', type: 'text', format: 'time' },
					长或短日期: { clazz: 'w80', type: 'text', format: 'datetime' },
					文件: { clazz: 'w80', type: 'file', bind: "PropertyA", serverUrl: '/Handler/NoAuthorityHandler.ashx', action: 'FileUpload' },
					_保存: { clazz: 'btn1 btnsave', call: 'idx.example.Func.Save();', verify: true }, //verify:true  点击该按钮需要验证表单
					_取消: { clazz: 'btn1 btncancel', call: ';' }
				}
			},
			Func: {
				Add: function () {
					ko.mapping.fromJS(new model(), idx.example.Entity);
					idx.example.Action = 'AddExample';
					Page.layer({ id: 'divForm', title: '添加 - 示例', width: '600px' });
				},
				Save: function () {
					alert("验证成功");
				}
			}
		};

		var Map = {
			CreateMap: function () {
				var map = new BMap.Map("map");
				map.enableScrollWheelZoom();   //启用滚轮放大缩小，默认禁用
				map.enableContinuousZoom();    //启用地图惯性拖拽，默认禁用
				JqPost(url_map, { action: "getAllDeviceData" }, function (data) {
					var b = true;
					if (data == [] || data.length == 0) {
						map.centerAndZoom("成都市", 11);
						return;
					}
					var arr = new Array();
					var imgUrl = "";
					for (var i = 0; i < data.length; i++) {
						if (data[i].AlarmState == 0) {
							imgUrl = "/Images/icons/green.png";  //正常
						} else if (data[i].AlarmState == 1) {
							imgUrl = "/Images/icons/yellow.png";  //告警
						} else {
							imgUrl = "/Images/icons/red.png";  //超标
						}
						var point = new BMap.Point(data[i].Longitude, data[i].Latitude);
						var marker = new BMap.Marker(point, { icon: new BMap.Icon(imgUrl ? imgUrl : '/Images/image_icon.png', new BMap.Size(30, 30)) });  // 创建标注
						arr[i] = data[i].Id;
						// 将标注添加到地图中
						if (b) {
							map.centerAndZoom(point, 14);
							b = false;
						}
						var opts = {
							width: 200,     // 信息窗口宽度
							height: 100,     // 信息窗口高度
							enableMessage: true,//设置允许信息窗发送短息
							message: ""
						}
						var infoWindow = new BMap.InfoWindow(data[i].name, opts);  // 创建信息窗口对象 
						marker.addEventListener("click", Map.Loca);
						var label = new BMap.Label(data[i].Name, { offset: new BMap.Size(20, -10) });
						marker.setLabel(label);
						map.addOverlay(marker);
					}
				});
			},
			Loca: function (e) {
				var p = e.target;
				var point = p.getPosition();
				JqPost(url_map, { action: "getAllDeviceData" }, function (data) {
					for (var i = 0; i < data.length; i++) {
						if (data[i].Longitude == point.lng && data[i].Latitude == point.lat) {
							p.removeEventListener("click", Map.Loca);
							location.href = "/Pages/AllCharts.html?id=" + data[i].Id;
						}
					}
				});
			},
		};

		$(function () {
			$.rtmc.list();
			$.rtmc.form();
			Map.CreateMap();
			idx.example.Pager.GetPagedData(1);
			$(".chosen-select").chosen({ search_contains: true, no_results_text: '没有匹配结果', width: '200px' });
		});
	</script>
</head>
<body>
	<div id="divTop" class="top" data-bind="template: 'top-template'"></div>
	<div id="divLeft" class="left" data-bind="template: 'left-template'"></div>
	<div id="divContent" class="content">
		<div class="tab">
			<div class="tab_title" onclick="Page.switchTab(this,'divIndex')">系统首页</div>
			<%--<div class="tab_title1" onclick="Page.switchTab(this,'divExample')">开发范例</div>
			<div class="tab_title1" onclick="Page.switchTab(this,'divLeftAndRight')">左右布局</div>--%>
		</div>

		<div id="divIndex" class="tab_content ">
			<div class="panel">
				<!--查询条-->
				<div class="querypanel">
					<ul>
						<li>关键字：<input type="text" id="txtKey" /></li>
						<li><a href="javascript:RTMC.Device.Pager.GetPagedData(1);" class="btn1 btnsearch">查询</a></li>
					</ul>
				</div>
			</div>
			<div id="map" style="position: absolute; height: 90%; width: 100%;"></div>
		</div>
		<div id="divExample" class="tab_content hide">
			<!--操作条-->
			<div class="panel">
				<!--查询条-->
				<div class="querypanel">
					<ul>
						<li>关键字：<input type="text" /></li>
						<li>
							<input type="text" onclick="layer.msg('layer.msg(提示信息)')" /><img class="choice" title="单击选择" src="/Images/icons/search2.png" onclick="layer.msg('点击了查询按钮')" />
						</li>
						<li id="liPick"></li>
						<li>
							<select>
								<option>--请选择--</option>
							</select>
						</li>
						<li>
							<select data-placeholder="--带检索功能的下拉框--" class="chosen-select">
								<option></option>
								<option>添加</option>
								<option>删除</option>
								<option>更新</option>
								<option>查询</option>
								<option>列表</option>
								<option>表单</option>
							</select>
						</li>
						<li><a href="javascript:;" class="btn1 btnsearch">查询</a></li>
					</ul>
				</div>

				<!--按钮条-->
				<div class="btnpanel">
					<ul>
						<li>
							<a href="javascript:;" class="btn1 btnsave">btn1</a>
						</li>
						<li>
							<a href="javascript:;" class="btn2 btnsave">btn2</a>
						</li>
						<li>
							<a href="javascript:;" class="btn3 btnrecycl">btn3</a>
						</li>
						<li>
							<a href="javascript:;" class="btn3 btnexit">btnexit</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnsave">btnsave</a>
						</li>
						<li>
							<a href="javascript:idx.example.Func.Add();" class="btn1 btnadd">btnadd</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnedit2">btnedit2</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnrecycl">btnrecycl</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btninfo">btninfo</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnview">btnview</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnarrag">btnarrag</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btncancel">btncancel</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnredo">btnredo</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnlist">btnlist</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnundo">btnundo</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnaudit">btnaudit</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnexcel">btnexcel</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btndel">btndel</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnedit">btnedit</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnfiles">btnfiles</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnsearch">btnsearch</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnhome">btnhome</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btndownload">btndownload</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnman">btnman</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnsubmit">btnsubmit</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnok">btnok</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnupdate">btnupdate</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnplay">btnplay</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnmonitor">btnmonitor</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btngoog">btngoog</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnall">btnall</a>
						</li>
						<li>
							<a href="javascript:;" class="btn1 btnreset">btnreset</a>
						</li>
					</ul>
				</div>
			</div>
			<div class="gray_list" data-list="idx.example"></div>
			<div class="pager" data-pager="idx.example"></div>
		</div>
		<div id="divLeftAndRight" class="tab_content hide">
			<div class="con_left">
				<div class="panel">
					<%--按钮条--%>
					<div class="btnpanel">
						<ul>
							<li>
								<a href="javascript:;" class="btn1 btnsave">btn1</a>
							</li>
						</ul>
					</div>

				</div>
				<div class="main_content">
					类容
				</div>
			</div>
			<div class="con_right">
				<!--操作条-->
				<div class="panel">
					<%--查询条--%>
					<div class="querypanel">
						<ul>
							<li>关键字：<input type="text" /></li>
							<li>
								<select>
									<option>请选择</option>
								</select>
							</li>
							<li>
								<input type="text" onclick="alert('s2')" /><img class="choice" title="单机选择" src="/Images/icons/search2.png" onclick="alert('s')" />
							</li>
							<li><a href="javascript:;" class="btn1 btnsearch">查询</a></li>
						</ul>
					</div>

					<%--按钮条--%>
					<div class="btnpanel">
						<ul>
							<li>
								<a href="javascript:;" class="btn1 btnsave">btn1</a>
							</li>

						</ul>
					</div>

				</div>
				<div class="gray_list">
					<table>
						<tr>
							<th class="leborno">编号</th>
							<th>姓名</th>
							<th>年龄</th>
							<th>性别</th>
							<th>生日</th>
							<th>操作</th>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
						<tr>
							<td class="leborno">编号</td>
							<td>马云</td>
							<td>38</td>
							<td>男</td>
							<td>1987-12-5</td>
							<td style="width: 15em">
								<a href="javascript:;" class="btn3 btnview">详情</a>
								<a href="javascript:;" class="btn3 btnedit2">编辑</a>
								<a href="javascript:;" class="btn3 btnrecycl">删除</a>
							</td>
						</tr>
					</table>
				</div>
				<div class="pager">
					<ul>
						<li><a href="javascript:;">首页</a></li>
						<li><a href="javascript:;" class="fybtn1" title="上一页">&nbsp;</a></li>
						<li>0/0页</li>
						<li><a href="javascript:;" class="fybtn2" title="下一页">&nbsp;</a></li>
						<li><a href="javascript:;">尾页</a></li>
						<li>跳转到第</li>
						<li>
							<input onkeyup="if(this.value.length==1){this.value=this.value.replace(/[^1-9]/g,'')}else{this.value=this.value.replace(/\D/g,'')}" type="text" id="txtGo" /></li>
						<li>页</li>
						<li><a href="javascript:;" class="btn3 fybtn3">转</a></li>
					</ul>
				</div>
			</div>
		</div>
	</div>
	<div id="divBottom" class="bottom" data-bind="template: 'bottom-template'"></div>

	<div id="divForm" class="layer" data-form="idx.example"></div>
</body>
</html>
