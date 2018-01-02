using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using Newtonsoft.Json;

namespace RTMC.Core {
	/// <summary>
	/// 数据类型转换，静态类
	/// </summary>
	public static class Parse {

		#region string
		/// <summary>转为字符串，若 obj 为 null，则返回 ""</summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public static string ToString(object obj) {
			if (obj == null) return "";
			return obj.ToString();
		}

		/// <summary>转为字符串，若 obj 为 null，则返回 null</summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public static string ToStringAble(object obj) {
			if (obj == null) return null;
			return obj.ToString();
		}

		/// <summary>转为字符串，若 obj 为 null或空，则返回默认值</summary>
		/// <param name="obj"></param>
		/// <param name="def">默认值</param>
		/// <returns></returns>
		public static string ToString(object obj, string def) {
			if (obj == null) return def;
			string rs = obj.ToString();
			if (rs == "") return def;
			return rs;
		}

		/// <summary>将已有相关类定义的实例转为 Json 字符串，并使用 Base64 编码。</summary>
		/// <param name="obj">相关类实例</param>
		/// <returns></returns>
		public static string ToBase64(object obj) {
			return Convert.ToBase64String(Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(obj)));
		}

		/// <summary>将 Base64 编码的 Json 字符串解析为相关类实例。</summary>
		/// <typeparam name="T">相关类</typeparam>
		/// <param name="obj">Base64 编码的 Json 字符串</param>
		/// <returns></returns>
		public static T FromBase64<T>(object obj) {
			return JsonConvert.DeserializeObject<T>(Encoding.UTF8.GetString(Convert.FromBase64String(Parse.ToString(obj))));
		}
		#endregion

		#region int
		/// <summary>尝试转为 int 型，若转换失败则返回 0</summary>
		/// <param name="obj">参数</param>
		/// <returns></returns>
		public static int ToInt(object obj) {
			int i = 0;
			ToInt(obj, out i);
			return i;
		}

		/// <summary>尝试转为 int 型，若转换失败则结果为 0</summary>
		/// <param name="obj">参数</param>
		/// <param name="msg">转换结果</param>
		/// <returns>失败原因</returns>
		public static string ToInt(object obj, out int result) {
			if (obj == null) {
				result = 0;
				return "参数obj为null。";
			} else {
				try {
					result = int.Parse(ToString(obj));
					return "";
				} catch (Exception e) {
					result = 0;
					return e.Message;
				}
			}
		}

		/// <summary>使用 int.TryParse 方法来尝试转换 int 型，若转换失败则返回 0</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static int TryInt(object obj) {
			int i = 0;
			int.TryParse(ToString(obj), out i);
			return i;
		}

		public static int TryInt(int? num) {
			return num.HasValue ? num.Value : 0;
		}

		public static int TryInt(int? num, int def) {
			return num.HasValue ? num.Value : def;
		}

		/// <summary>使用 int.TryParse 方法来尝试转换 int 类型，若转换失败则返回默认值</summary>
		/// <param name="obj">待转换参数</param>
		/// <param name="def">默认值</param>
		/// <returns></returns>
		public static int TryInt(object obj, int def) {
			if (obj == null)
				return def;
			int.TryParse(ToString(obj), out def);
			return def;
		}

		/// <summary>使用 int.TryParse 方法来尝试转为 int? 型，若 obj 为 null，则返回 null 值</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static int? TryIntAble(object obj) {
			if (obj == null) return null;
			int i = 0;
			if (int.TryParse(ToString(obj), out i)) {
				return i;
			} else {
				return null;
			}
		}
		#endregion

		#region long
		/// <summary>尝试转为 long 型，若转换失败则返回 0</summary>
		/// <param name="obj">参数</param>
		/// <returns></returns>
		public static long ToLong(object obj) {
			long i = 0;
			ToLong(obj, out i);
			return i;
		}

		/// <summary>尝试转为 long 型，若转换失败则结果为 0</summary>
		/// <param name="obj">参数</param>
		/// <param name="msg">转换结果</param>
		/// <returns>失败原因</returns>
		public static string ToLong(object obj, out long result) {
			if (obj == null) {
				result = 0;
				return "参数obj为null。";
			} else {
				try {
					result = long.Parse(ToString(obj));
					return "";
				} catch (Exception e) {
					result = 0;
					return e.Message;
				}
			}
		}

		/// <summary>使用 long.TryParse 方法来尝试转换 long 型，若转换失败则返回 0</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static long TryLong(object obj) {
			long i = 0;
			long.TryParse(ToString(obj), out i);
			return i;
		}

		public static long TryLong(long? num) {
			return num.HasValue ? num.Value : 0;
		}

		public static long TryLong(long? num, long def) {
			return num.HasValue ? num.Value : def;
		}

		/// <summary>使用 long.TryParse 方法来尝试转换 long 类型，若转换失败则返回默认值</summary>
		/// <param name="obj">待转换参数</param>
		/// <param name="def">默认值</param>
		/// <returns></returns>
		public static long TryLong(object obj, long def) {
			if (obj == null)
				return def;
			long.TryParse(ToString(obj), out def);
			return def;
		}

		/// <summary>使用 long.TryParse 方法来尝试转为 long? 型，若 obj 为 null，则返回 null 值</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static long? TryLongAble(object obj) {
			if (obj == null) return null;
			long i = 0;
			if (long.TryParse(ToString(obj), out i)) {
				return i;
			} else {
				return null;
			}
		}
		#endregion


		#region double
		/// <summary>尝试转为 double 型，若转换失败则返回 0</summary>
		/// <param name="obj">参数</param>
		/// <returns></returns>
		public static double ToDouble(object obj) {
			double i = 0.00;
			ToDouble(obj, out i);
			return i;
		}

		/// <summary>尝试转为 double 型，若转换失败则结果为 0</summary>
		/// <param name="obj">参数</param>
		/// <param name="msg">转换结果</param>
		/// <returns>失败原因</returns>
		public static string ToDouble(object obj, out double result) {
			if (obj == null) {
				result = 0.00;
				return "参数obj为null。";
			} else {
				try {
					result = double.Parse(ToString(obj));
					return "";
				} catch (Exception e) {
					result = 0.00;
					return e.Message;
				}
			}
		}

		/// <summary>使用 double.TryParse 方法来尝试转换 double 型，若转换失败则返回 0</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static double TryDouble(object obj) {
			double i = 0.00;
			double.TryParse(ToString(obj), out i);
			return i;
		}

		/// <summary>使用 double.TryParse 方法来尝试转换 double 类型，若转换失败则返回默认值</summary>
		/// <param name="obj">待转换参数</param>
		/// <param name="def">默认值</param>
		/// <returns></returns>
		public static double TryDouble(object obj, double def) {
			if (obj == null)
				return def;
			double.TryParse(ToString(obj), out def);
			return def;
		}

		/// <summary>使用 double.TryParse 方法来尝试转为 double? 型，若 obj 为 null，则返回 null 值</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static double? TryDoubleAble(object obj) {
			if (obj == null) return null;
			double i = 0.00;
			if (double.TryParse(ToString(obj), out i)) {
				return i;
			} else {
				return null;
			}
		}
		#endregion

		#region decimal
		/// <summary>尝试转为 decimal 型，若转换失败则返回 0</summary>
		/// <param name="obj">参数</param>
		/// <returns></returns>
		public static decimal ToDecimal(object obj) {
			decimal i = 0.00m;
			ToDecimal(obj, out i);
			return i;
		}

		/// <summary>尝试转为 decimal 型，若转换失败则结果为 0</summary>
		/// <param name="obj">参数</param>
		/// <param name="msg">转换结果</param>
		/// <returns>失败原因</returns>
		public static string ToDecimal(object obj, out decimal result) {
			if (obj == null) {
				result = 0.00m;
				return "参数obj为null。";
			} else {
				try {
					result = decimal.Parse(ToString(obj));
					return "";
				} catch (Exception e) {
					result = 0.00m;
					return e.Message;
				}
			}
		}

		/// <summary>使用 decimal.TryParse 方法来尝试转换 decimal 型，若转换失败则返回 0</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static decimal TryDecimal(object obj) {
			decimal i = 0.00m;
			decimal.TryParse(ToString(obj), out i);
			return i;
		}

		/// <summary>使用 decimal.TryParse 方法来尝试转换 decimal 类型，若转换失败则返回默认值</summary>
		/// <param name="obj">待转换参数</param>
		/// <param name="def">默认值</param>
		/// <returns></returns>
		public static decimal TryDecimal(object obj, decimal def) {
			if (obj == null)
				return def;
			decimal.TryParse(ToString(obj), out def);
			return def;
		}

		/// <summary>使用 decimal.TryParse 方法来尝试转为 decimal? 型，若 obj 为 null，则返回 null 值</summary>
		/// <param name="obj">待转换参数</param>
		/// <returns></returns>
		public static decimal? TryDecimalAble(object obj) {
			if (obj == null) return null;
			decimal i = 0.00m;
			if (decimal.TryParse(ToString(obj), out i)) {
				return i;
			} else {
				return null;
			}
		}
		#endregion

		#region bool
		/// <summary>尝试转为 bool 型</summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public static bool ToBool(object obj) {
			bool i = false;
			ToBool(obj, out i);
			return i;
		}

		/// <summary>尝试转为 bool 型，并返回失败原因</summary>
		/// <param name="obj">待转换参数</param>
		/// <param name="result">转换结果</param>
		/// <returns>失败原因</returns>
		public static string ToBool(object obj, out bool result) {
			if (obj == null) {
				result = false;
				return "参数obj为null";
			} else {
				try {
					result = bool.Parse(obj.ToString());
					return "";
				} catch (Exception e) {
					result = false;
					return e.Message;
				}
			}
		}

		/// <summary>使用 bool.TryParse 方法来尝试转换 bool 型</summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public static bool TryBool(object obj) {
			int n = 0;
			if (int.TryParse(ToString(obj), out n)) {
				return n > 0;
			}
			bool i = false;
			bool.TryParse(ToString(obj), out i);
			return i;
		}

		/// <summary>
		/// 使用 bool.TryParse 方法来尝试转换 bool 型。
		/// </summary>
		/// <param name="obj"></param>
		/// <param name="def"></param>
		/// <returns></returns>
		public static bool TryBool(object obj, bool def) {
			int n = 0;
			if (int.TryParse(ToString(obj), out n)) {
				return n > 0;
			}
			bool i = false;
			if (bool.TryParse(ToString(obj), out i)) {
				return i;
			} else {
				return def;
			}
		}

		/// <summary>使用 boo.TryParse 方法来尝试转换为 bool? 型，若 obj 为 null，则返回 null 值。</summary>
		/// <param name="obj">待转换的参数。</param>
		/// <returns></returns>
		public static bool? TryBoolAble(object obj) {
			if (obj == null) return null;
			int n = 0;
			if (int.TryParse(ToString(obj), out n)) {
				return n > 0;
			}
			bool i = false;
			if (bool.TryParse(ToString(obj), out i)) {
				return i;
			} else {
				return null;
			}
		}
		#endregion

		#region DateTime
		/// <summary>使用 DateTime.TryParse 方法来尝试转为 DateTime 型，若失败则返回 1900-01-01 00:00:00</summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public static DateTime TryDateTime(object obj) {
			DateTime i = new DateTime();
			bool rs = DateTime.TryParse(ToString(obj), out i);
			return rs ? i : new DateTime(1900, 1, 1, 0, 0, 0);
		}

		/// <summary>使用 DateTime.TryParse 方法来尝试转为 DateTime 型，若失败则返回默认值</summary>
		/// <param name="obj"></param>
		/// <param name="def">默认值</param>
		/// <returns></returns>
		public static DateTime TryDateTime(object obj, DateTime def) {
			DateTime i = new DateTime();
			bool rs = DateTime.TryParse(ToString(obj), out i);
			return rs ? i : def;
		}

		/// <summary>使用 DateTime.TryParse 方法来尝试转为 DateTime? 型，若失败，则返回 null 值</summary>
		/// <param name="obj">待转换的参数</param>
		/// <returns></returns>
		public static DateTime? TryDateTimeAble(object obj) {
			if (obj == null) return null;
			DateTime i = new DateTime();
			bool rs = DateTime.TryParse(ToString(obj), out i);
			return rs ? new DateTime?(i) : null;
		}
		#endregion

		public static Guid TryGuid(object obj) {
			Guid i = Guid.Empty;
			Guid.TryParse(ToString(obj), out i);
			return i;
		}
	}
}
