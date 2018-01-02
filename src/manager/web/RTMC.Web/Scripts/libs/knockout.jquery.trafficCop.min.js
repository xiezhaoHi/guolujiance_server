/*
	TrafficCop by Jim Cowart
	License: Dual licensed MIT (http://www.opensource.org/licenses/mit-license) & GPL (http://www.opensource.org/licenses/gpl-license)
	Version 0.3.0
*/
(function (e, t) { var n = {}; e.trafficCop = function (t, r) { var s = t, o; if (arguments.length === 2) { s = e.extend(true, r, { url: t }) } o = JSON.stringify(s); if (o in n) { for (i in { success: 1, error: 1, complete: 1 }) { n[o][i](s[i]) } } else { n[o] = e.ajax(s).always(function () { delete n[o] }) } return n[o] } })(jQuery);