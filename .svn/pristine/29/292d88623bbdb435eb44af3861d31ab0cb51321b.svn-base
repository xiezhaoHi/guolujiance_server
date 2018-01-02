/// <reference path="../libs/jquery-1.10.2.min.js" />
/// <reference path="/Scripts/libs/json2.min.js" />

// 强化标准JS对象
(function (window) {
	'use strict';
	//-- WARNING! rewrite default method!
	Array.prototype.find = function (valueOrPredicate) {
		var predicate = typeof valueOrPredicate == "function" ? valueOrPredicate : function (value) { return value === valueOrPredicate; };
		for (var i = 0, value; value = this[i]; i++) {
			if (predicate(value)) {
				return value;
			}
		}
		return -1;
	};
	Array.prototype.findLast = function (valueOrPredicate) {
		var predicate = typeof valueOrPredicate == "function" ? valueOrPredicate : function (value) { return value === valueOrPredicate; };
		for (var i = this.length - 1, value; value = this[i]; i--) {
			if (predicate(value)) {
				return value;
			}
		}
		return -1;
	};
	Array.prototype.indexOf = function (value) {
		/// <summary>查找对象在数组中首先出现的位置。可设置第二个参数(String)，用以指定比较对象的主键，例如：arrUsers.indexOf('张三','Name')</summary>
		/// <param name="value" type="Object">待查找的对象或待查找属性值。</param>
		if (arguments.length === 1) {
			for (var i = 0, j; j = this[i]; i++) {
				if (j == value) { return i; }
			}
		}
		if (arguments.length === 2) {
			var key = arguments[1];
			for (var i = 0, j; j = this[i]; i++) {
				if (j[key] === value) { return i; }
			}
		}
		return -1;
	}
	Array.prototype.lastIndexOf = function (e) {
		/// <summary>重写方法，查找对象在数组中最后出现的位置。</summary>
		/// <param name="e" type="Object">待查找的对象。</param>
		if (arguments.length === 1) {
			for (var i = this.length - 1, j; j = this[i]; i--) {
				if (j == e) { return i; }
			}
		}
		if (arguments.length === 2) {
			var key = arguments[1];
			for (var i = this.length - 1, j; j = this[i]; i--) {
				if (j[key] === value) { return i; }
			}
		}
		return -1;
	}
	Array.prototype.moveUp = function (index) {
		/// <summary>上移数组中的指定元素。</summary>
		/// <param name="index" type="Integer">指定元素的索引。</param>
		index = Math.floor(index);
		// (0, n], n = this.length - 1;
		if (index > 0 && index <= this.length - 1) {
			var tmpA = JSON.stringify(this[index]);
			var tmpB = JSON.stringify(this[index - 1]);
			this[index] = JSON.parse(tmpB);
			this[index - 1] = JSON.parse(tmpA);
			//var arr = [];
			//for (var i = 0, j; j = this[i]; i++) {
			//	if (i === index || i === index - 1) continue;
			//	arr[i] = this[i];
			//}
			//arr[index] = this[index - 1];
			//arr[index - 1] = this[index];
			//this = arr;
		}
	};
	Array.prototype.moveDown = function (index) {
		/// <summary>上移数组中的指定元素。</summary>
		/// <param name="index" type="Integer">指定元素的索引。</param>
		index = Math.floor(index);
		// [0, n), n = this.length - 1;
		if (index >= 0 && index < this.length - 1) {
			var tmpA = JSON.stringify(this[index]);
			var tmpB = JSON.stringify(this[index + 1]);
			this[index] = JSON.parse(tmpB);
			this[index + 1] = JSON.parse(tmpA);
			//var arr = [];
			//for (var i = 0, j; j = this[i]; i++) {
			//	if (i === index || i === index + 1) continue;
			//	arr[i] = this[i];
			//}
			//arr[index] = this[index + 1];
			//arr[index + 1] = this[index];
			//this = arr;
		}
	};
	Array.prototype.moveTop = function (index) {
		var tmp = this[indx];
		this.splice(index, 1);
		this.unshift(tmp);
	};
	Array.prototype.moveBottom = function (index) {
		var tmp = this[index];
		this.splice(index, 1);
		this.push(tmp);
	};

	String.prototype.dataLength = function () {
		/// <summary>获取数值长度。</summary>
		var len = 0;
		for (var i = 0, j; j = this[i]; i++) {
			var char = encodeURI(j);
			len += char.length === 1 ? 1 : char.length / 3;
		}
		return len;
	}
	String.prototype.sqlLength = function () {
		/// <summary>获取存储长度。</summary>
		var len = 0;
		for (var i = 0, j; j = this[i]; i++) {
			var code = j.charCodeAt(0);
			len += code > 255 ? 2 : 1;
		}
		return len;
	}

	// From https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/keys
	// IE8 support
	if (!Object.keys) {
		Object.keys = (function () {
			'use strict';
			var hasOwnProperty = Object.prototype.hasOwnProperty,
				hasDontEnumBug = !({ toString: null }).propertyIsEnumerable('toString'),
				dontEnums = [
				  'toString',
				  'toLocaleString',
				  'valueOf',
				  'hasOwnProperty',
				  'isPrototypeOf',
				  'propertyIsEnumerable',
				  'constructor'
				],
				dontEnumsLength = dontEnums.length;

			return function (obj) {
				if (typeof obj !== 'object' && (typeof obj !== 'function' || obj === null)) {
					throw new TypeError('Object.keys called on non-object');
				}

				var result = [], prop, i;

				for (prop in obj) {
					if (hasOwnProperty.call(obj, prop)) {
						result.push(prop);
					}
				}

				if (hasDontEnumBug) {
					for (i = 0; i < dontEnumsLength; i++) {
						if (hasOwnProperty.call(obj, dontEnums[i])) {
							result.push(dontEnums[i]);
						}
					}
				}
				return result;
			};
		}());
	}

	window.timeoutList = [];
	window.intervalList = [];

	window.oldSetTimeout = window.setTimeout;
	window.oldSetInterval = window.setInterval;
	window.oldClearTimeout = window.clearTimeout;
	window.oldClearInterval = window.clearInterval;

	window.setTimeout = function (code, delay) {
		var retval = window.oldSetTimeout(code, delay);
		window.timeoutList.push(retval);
		return retval;
	};
	window.clearTimeout = function (id) {
		var ind = window.timeoutList.indexOf(id);
		if (ind >= 0) {
			window.timeoutList.splice(ind, 1);
		}
		var retval = window.oldClearTimeout(id);
		return retval;
	};
	window.setInterval = function (code, delay) {
		var retval = window.oldSetInterval(code, delay);
		window.intervalList.push(retval);
		return retval;
	};
	window.clearInterval = function (id) {
		var ind = window.intervalList.indexOf(id);
		if (ind >= 0) {
			window.intervalList.splice(ind, 1);
		}
		var retval = window.oldClearInterval(id);
		return retval;
	};
	window.clearAllTimeouts = function () {
		for (var i in window.timeoutList) {
			window.oldClearTimeout(window.timeoutList[i]);
		}
		window.timeoutList = [];
	};
	window.clearAllIntervals = function () {
		for (var i in window.intervalList) {
			window.oldClearInterval(window.intervalList[i]);
		}
		window.intervalList = [];
	};
})(window, undefined);

// 通用工具
(function (window) {
	'use strict';
	if (!window.Util) window.Util = {};

	if (typeof Util.numf !== 'function') {
		Util.numf = function (num, pattern) {
			/// <summary>格式化数字，例如 numf(12345.999,'#,##0.00'); numf(12345.999,'#,##0.##'); numf(123,'000000');</summary>
			/// <param name="num" type="Int">待格式化输出的数字</param>
			/// <param name="pattern" type="String">字符串格式</param>
			var strarr = num ? num.toString().split('.') : ['0'];
			var fmtarr = pattern ? pattern.split('.') : [''];
			var retstr = '';

			var str = strarr[0];
			var fmt = fmtarr[0];
			var i = str.length - 1;
			var comma = false;
			for (var f = fmt.length - 1; f >= 0; f--) {
				switch (fmt.substr(f, 1)) {
					case '#':
						if (i >= 0) retstr = str.substr(i--, 1) + retstr;
						break;
					case '0':
						if (i >= 0) retstr = str.substr(i--, 1) + retstr;
						else retstr = '0' + retstr;
						break;
					case ',':
						comma = true;
						retstr = ',' + retstr;
						break;
				}
			}
			if (i >= 0) {
				if (comma) {
					var l = str.length;
					for (; i >= 0; i--) {
						retstr = str.substr(i, 1) + retstr;
						if (i > 0 && ((l - i) % 3) == 0) retstr = ',' + retstr;
					}
				}
				else retstr = str.substr(0, i + 1) + retstr;
			}

			retstr = retstr + '.';
			str = strarr.length > 1 ? strarr[1] : '';
			fmt = fmtarr.length > 1 ? fmtarr[1] : '';
			i = 0;
			for (f = 0; f < fmt.length; f++) {
				switch (fmt.substr(f, 1)) {
					case '#':
						if (i < str.length) retstr += str.substr(i++, 1);
						break;
					case '0':
						if (i < str.length) retstr += str.substr(i++, 1);
						else retstr += '0';
						break;
				}
			}
			return retstr.replace(/^,+/, '').replace(/\.$/, '');
		};
	}

	if (typeof Util.guid !== 'function') {
		Util.guid = function () {
			/// <summary>生成唯一标识。</summary>
			var g = "";
			for (var i = 0; i < 32; i++) {
				g += Math.floor(Math.random() * 0xF).toString(0xF) + (i == 7 || i == 11 || i == 15 || i == 19 ? "-" : "");
			}
			return g;
		}
	}

	if (typeof Util.interceptor != 'function') {
		Util.interceptor = function (str, leg) {
			return leg > 1 && str.length > leg ? str.substr(0, leg - 1) + '…' : str;
		}
	}
})(window);

(function (window) {
	/*
	 * A JavaScript implementation of the Secure Hash Algorithm, SHA-1, as defined
	 * in FIPS 180-1
	 * Version 2.2 Copyright Paul Johnston 2000 - 2009.
	 * Other contributors: Greg Holt, Andrew Kepert, Ydnar, Lostinet
	 * Distributed under the BSD License
	 * See http://pajhome.org.uk/crypt/md5 for details.
	 */

	/*
	 * Configurable variables. You may need to tweak these to be compatible with
	 * the server-side, but the defaults work in most cases.
	 */
	var hexcase = 0;  /* hex output format. 0 - lowercase; 1 - uppercase        */
	var b64pad = ''; /* base-64 pad character. '=' for strict RFC compliance   */

	/*
	 * These are the functions you'll usually want to call
	 * They take string arguments and return either hex or base-64 encoded strings
	 */

	window.Hex_Sha1 = hex_sha1;

	function hex_sha1(s) { return rstr2hex(rstr_sha1(str2rstr_utf8(s))); }
	function b64_sha1(s) { return rstr2b64(rstr_sha1(str2rstr_utf8(s))); }
	function any_sha1(s, e) { return rstr2any(rstr_sha1(str2rstr_utf8(s)), e); }
	function hex_hmac_sha1(k, d) { return rstr2hex(rstr_hmac_sha1(str2rstr_utf8(k), str2rstr_utf8(d))); }
	function b64_hmac_sha1(k, d) { return rstr2b64(rstr_hmac_sha1(str2rstr_utf8(k), str2rstr_utf8(d))); }
	function any_hmac_sha1(k, d, e) { return rstr2any(rstr_hmac_sha1(str2rstr_utf8(k), str2rstr_utf8(d)), e); }

	/*
	 * Perform a simple self-test to see if the VM is working
	 */
	function sha1_vm_test() {
		return hex_sha1('abc').toLowerCase() == 'a9993e364706816aba3e25717850c26c9cd0d89d';
	}

	/*
	 * Calculate the SHA1 of a raw string
	 */
	function rstr_sha1(s) {
		return binb2rstr(binb_sha1(rstr2binb(s), s.length * 8));
	}

	/*
	 * Calculate the HMAC-SHA1 of a key and some data (raw strings)
	 */
	function rstr_hmac_sha1(key, data) {
		var bkey = rstr2binb(key);
		if (bkey.length > 16) bkey = binb_sha1(bkey, key.length * 8);

		var ipad = Array(16), opad = Array(16);
		for (var i = 0; i < 16; i++) {
			ipad[i] = bkey[i] ^ 0x36363636;
			opad[i] = bkey[i] ^ 0x5C5C5C5C;
		}

		var hash = binb_sha1(ipad.concat(rstr2binb(data)), 512 + data.length * 8);
		return binb2rstr(binb_sha1(opad.concat(hash), 512 + 160));
	}

	/*
	 * Convert a raw string to a hex string
	 */
	function rstr2hex(input) {
		try { hexcase } catch (e) { hexcase = 0; }
		var hex_tab = hexcase ? '0123456789ABCDEF' : '0123456789abcdef';
		var output = '';
		var x;
		for (var i = 0; i < input.length; i++) {
			x = input.charCodeAt(i);
			output += hex_tab.charAt((x >>> 4) & 0x0F)
				   + hex_tab.charAt(x & 0x0F);
		}
		return output;
	}

	/*
	 * Convert a raw string to a base-64 string
	 */
	function rstr2b64(input) {
		try { b64pad } catch (e) { b64pad = ''; }
		var tab = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
		var output = '';
		var len = input.length;
		for (var i = 0; i < len; i += 3) {
			var triplet = (input.charCodeAt(i) << 16)
						| (i + 1 < len ? input.charCodeAt(i + 1) << 8 : 0)
						| (i + 2 < len ? input.charCodeAt(i + 2) : 0);
			for (var j = 0; j < 4; j++) {
				if (i * 8 + j * 6 > input.length * 8) output += b64pad;
				else output += tab.charAt((triplet >>> 6 * (3 - j)) & 0x3F);
			}
		}
		return output;
	}

	/*
	 * Convert a raw string to an arbitrary string encoding
	 */
	function rstr2any(input, encoding) {
		var divisor = encoding.length;
		var remainders = Array();
		var i, q, x, quotient;

		/* Convert to an array of 16-bit big-endian values, forming the dividend */
		var dividend = Array(Math.ceil(input.length / 2));
		for (i = 0; i < dividend.length; i++) {
			dividend[i] = (input.charCodeAt(i * 2) << 8) | input.charCodeAt(i * 2 + 1);
		}

		/*
		 * Repeatedly perform a long division. The binary array forms the dividend,
		 * the length of the encoding is the divisor. Once computed, the quotient
		 * forms the dividend for the next step. We stop when the dividend is zero.
		 * All remainders are stored for later use.
		 */
		while (dividend.length > 0) {
			quotient = Array();
			x = 0;
			for (i = 0; i < dividend.length; i++) {
				x = (x << 16) + dividend[i];
				q = Math.floor(x / divisor);
				x -= q * divisor;
				if (quotient.length > 0 || q > 0)
					quotient[quotient.length] = q;
			}
			remainders[remainders.length] = x;
			dividend = quotient;
		}

		/* Convert the remainders to the output string */
		var output = '';
		for (i = remainders.length - 1; i >= 0; i--)
			output += encoding.charAt(remainders[i]);

		/* Append leading zero equivalents */
		var full_length = Math.ceil(input.length * 8 /
										  (Math.log(encoding.length) / Math.log(2)))
		for (i = output.length; i < full_length; i++)
			output = encoding[0] + output;

		return output;
	}

	/*
	 * Encode a string as utf-8.
	 * For efficiency, this assumes the input is valid utf-16.
	 */
	function str2rstr_utf8(input) {
		var output = '';
		var i = -1;
		var x, y;

		while (++i < input.length) {
			/* Decode utf-16 surrogate pairs */
			x = input.charCodeAt(i);
			y = i + 1 < input.length ? input.charCodeAt(i + 1) : 0;
			if (0xD800 <= x && x <= 0xDBFF && 0xDC00 <= y && y <= 0xDFFF) {
				x = 0x10000 + ((x & 0x03FF) << 10) + (y & 0x03FF);
				i++;
			}

			/* Encode output as utf-8 */
			if (x <= 0x7F)
				output += String.fromCharCode(x);
			else if (x <= 0x7FF)
				output += String.fromCharCode(0xC0 | ((x >>> 6) & 0x1F),
											  0x80 | (x & 0x3F));
			else if (x <= 0xFFFF)
				output += String.fromCharCode(0xE0 | ((x >>> 12) & 0x0F),
											  0x80 | ((x >>> 6) & 0x3F),
											  0x80 | (x & 0x3F));
			else if (x <= 0x1FFFFF)
				output += String.fromCharCode(0xF0 | ((x >>> 18) & 0x07),
											  0x80 | ((x >>> 12) & 0x3F),
											  0x80 | ((x >>> 6) & 0x3F),
											  0x80 | (x & 0x3F));
		}
		return output;
	}

	/*
	 * Encode a string as utf-16
	 */
	function str2rstr_utf16le(input) {
		var output = '';
		for (var i = 0; i < input.length; i++)
			output += String.fromCharCode(input.charCodeAt(i) & 0xFF,
										  (input.charCodeAt(i) >>> 8) & 0xFF);
		return output;
	}

	function str2rstr_utf16be(input) {
		var output = '';
		for (var i = 0; i < input.length; i++)
			output += String.fromCharCode((input.charCodeAt(i) >>> 8) & 0xFF,
										   input.charCodeAt(i) & 0xFF);
		return output;
	}

	/*
	 * Convert a raw string to an array of big-endian words
	 * Characters >255 have their high-byte silently ignored.
	 */
	function rstr2binb(input) {
		var output = Array(input.length >> 2);
		for (var i = 0; i < output.length; i++)
			output[i] = 0;
		for (var i = 0; i < input.length * 8; i += 8)
			output[i >> 5] |= (input.charCodeAt(i / 8) & 0xFF) << (24 - i % 32);
		return output;
	}

	/*
	 * Convert an array of big-endian words to a string
	 */
	function binb2rstr(input) {
		var output = '';
		for (var i = 0; i < input.length * 32; i += 8)
			output += String.fromCharCode((input[i >> 5] >>> (24 - i % 32)) & 0xFF);
		return output;
	}

	/*
	 * Calculate the SHA-1 of an array of big-endian words, and a bit length
	 */
	function binb_sha1(x, len) {
		/* append padding */
		x[len >> 5] |= 0x80 << (24 - len % 32);
		x[((len + 64 >> 9) << 4) + 15] = len;

		var w = Array(80);
		var a = 1732584193;
		var b = -271733879;
		var c = -1732584194;
		var d = 271733878;
		var e = -1009589776;

		for (var i = 0; i < x.length; i += 16) {
			var olda = a;
			var oldb = b;
			var oldc = c;
			var oldd = d;
			var olde = e;

			for (var j = 0; j < 80; j++) {
				if (j < 16) w[j] = x[i + j];
				else w[j] = bit_rol(w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16], 1);
				var t = safe_add(safe_add(bit_rol(a, 5), sha1_ft(j, b, c, d)),
								 safe_add(safe_add(e, w[j]), sha1_kt(j)));
				e = d;
				d = c;
				c = bit_rol(b, 30);
				b = a;
				a = t;
			}

			a = safe_add(a, olda);
			b = safe_add(b, oldb);
			c = safe_add(c, oldc);
			d = safe_add(d, oldd);
			e = safe_add(e, olde);
		}
		return Array(a, b, c, d, e);

	}

	/*
	 * Perform the appropriate triplet combination function for the current
	 * iteration
	 */
	function sha1_ft(t, b, c, d) {
		if (t < 20) return (b & c) | ((~b) & d);
		if (t < 40) return b ^ c ^ d;
		if (t < 60) return (b & c) | (b & d) | (c & d);
		return b ^ c ^ d;
	}

	/*
	 * Determine the appropriate additive constant for the current iteration
	 */
	function sha1_kt(t) {
		return (t < 20) ? 1518500249 : (t < 40) ? 1859775393 :
			   (t < 60) ? -1894007588 : -899497514;
	}

	/*
	 * Add integers, wrapping at 2^32. This uses 16-bit operations internally
	 * to work around bugs in some JS interpreters.
	 */
	function safe_add(x, y) {
		var lsw = (x & 0xFFFF) + (y & 0xFFFF);
		var msw = (x >> 16) + (y >> 16) + (lsw >> 16);
		return (msw << 16) | (lsw & 0xFFFF);
	}

	/*
	 * Bitwise rotate a 32-bit number to the left.
	 */
	function bit_rol(num, cnt) {
		return (num << cnt) | (num >>> (32 - cnt));
	}
})(window);

(function (window) {
	if (!window.Base64) {
		window.Base64 = function () {
			// private property
			_keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

			// public method for encoding
			this.encode = function (input) {
				var output = "";
				var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
				var i = 0;
				input = _utf8_encode(input);
				while (i < input.length) {
					chr1 = input.charCodeAt(i++);
					chr2 = input.charCodeAt(i++);
					chr3 = input.charCodeAt(i++);
					enc1 = chr1 >> 2;
					enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
					enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
					enc4 = chr3 & 63;
					if (isNaN(chr2)) {
						enc3 = enc4 = 64;
					} else if (isNaN(chr3)) {
						enc4 = 64;
					}
					output = output +
					_keyStr.charAt(enc1) + _keyStr.charAt(enc2) +
					_keyStr.charAt(enc3) + _keyStr.charAt(enc4);
				}
				return output;
			}

			// public method for decoding
			this.decode = function (input) {
				var output = "";
				var chr1, chr2, chr3;
				var enc1, enc2, enc3, enc4;
				var i = 0;
				input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");
				while (i < input.length) {
					enc1 = _keyStr.indexOf(input.charAt(i++));
					enc2 = _keyStr.indexOf(input.charAt(i++));
					enc3 = _keyStr.indexOf(input.charAt(i++));
					enc4 = _keyStr.indexOf(input.charAt(i++));
					chr1 = (enc1 << 2) | (enc2 >> 4);
					chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
					chr3 = ((enc3 & 3) << 6) | enc4;
					output = output + String.fromCharCode(chr1);
					if (enc3 != 64) {
						output = output + String.fromCharCode(chr2);
					}
					if (enc4 != 64) {
						output = output + String.fromCharCode(chr3);
					}
				}
				output = _utf8_decode(output);
				return output;
			}

			// private method for UTF-8 encoding
			_utf8_encode = function (string) {
				string = string.replace(/\r\n/g, "\n");
				var utftext = "";
				for (var n = 0; n < string.length; n++) {
					var c = string.charCodeAt(n);
					if (c < 128) {
						utftext += String.fromCharCode(c);
					} else if ((c > 127) && (c < 2048)) {
						utftext += String.fromCharCode((c >> 6) | 192);
						utftext += String.fromCharCode((c & 63) | 128);
					} else {
						utftext += String.fromCharCode((c >> 12) | 224);
						utftext += String.fromCharCode(((c >> 6) & 63) | 128);
						utftext += String.fromCharCode((c & 63) | 128);
					}

				}
				return utftext;
			}

			// private method for UTF-8 decoding
			_utf8_decode = function (utftext) {
				var string = "";
				var i = 0;
				var c = c1 = c2 = 0;
				while (i < utftext.length) {
					c = utftext.charCodeAt(i);
					if (c < 128) {
						string += String.fromCharCode(c);
						i++;
					} else if ((c > 191) && (c < 224)) {
						c2 = utftext.charCodeAt(i + 1);
						string += String.fromCharCode(((c & 31) << 6) | (c2 & 63));
						i += 2;
					} else {
						c2 = utftext.charCodeAt(i + 1);
						c3 = utftext.charCodeAt(i + 2);
						string += String.fromCharCode(((c & 15) << 12) | ((c2 & 63) << 6) | (c3 & 63));
						i += 3;
					}
				}
				return string;
			}
		};
	}
})(window);

(function (window) {
	if (!window.request) {
		window.request = {};
		var params = location.search;
		if (params.indexOf('?') != -1) {
			var str = params.substr(1);
			var arr = str.split('&');
			for (var i = 0; i < arr.length; i++) {
				var kv = arr[i].split('=');
				window.request[kv[0]] = decodeURIComponent(kv[1]);
			}
		}
	}
})(window);

function JqPost(controller, data, success, error, config) {
	/// <summary>封装 Jquery Ajax 方法。</summary>
	/// <param name="controller" type="String">coltroller名称。</param>
	/// <param name="action" type="String">action名称。</param>
	/// <param name="data" type="Object">POST 数据包。</param>
	/// <param name="success" type="Function">请求执行成功时执行的方法。</param>
	/// <param name="error" type="Function">请求错误时执行的方法。</param>
	/// <param name="config" type="json">{cache:bool,async:bool}</param>
	/// cache:是否启用本地缓存（默认为不启用），async：提交方式:true异步提交，false同步提交
	var settings = { type: 'POST', dataType: 'json', data: data, async: true };
	var loadi = layer.load(2);//'加载中...'
	if (typeof data !== 'function')
		var key = Hex_Sha1(JSON.stringify(data));
	//处理成功后回调函数
	if (localStorage && config && config.cache && success) { //启用缓存时,封装回调函数
		var value = JSON.parse(localStorage.getItem(key));
		if (value) {
			success(value); //缓存中存在结果是，直接返回结果
			return;
		} else {
			settings.success = function (data) {       ///缓存中未存在结果是，直接向服务器提交请求
				localStorage.setItem(key, JSON.stringify(data));
				layer.close(loadi);
				success(data);
			};
		}
	} else {
		settings.success = function (data) //未启用缓存是，封装success回调函数
		{
			layer.close(loadi);
			success(data);
		}
	}
	settings.error = function (e) {
		layer.close(loadi);
		if (e.responseText === "")
			layer.alert("&nbsp;&nbsp;无效请求！&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", { icon: 5 });
		else if (e.responseText === "请重新登陆！") {
			window.location.href = "/Login.aspx?ReturnUrl=" + location.pathname;
		}
		else if (typeof error === "function")
			error(e);
		else
			layer.alert(e.responseText, { icon: 5 });
	}
	if (localStorage && config && !config.cache) {
		localStorage.removeItem(key);
	}
	if (config && config.async == false) {
		settings.async = false;
	}
	$.ajax(controller, settings);
}

