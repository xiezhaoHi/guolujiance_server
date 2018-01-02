using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Reflection;
using System.Runtime.Serialization.Formatters.Binary;
using System.Security.Cryptography;
using System.Text;
using System.Web;

namespace RTMC.Core {
	public class Function {
		private static Random rand = new Random();

		public static string GetRandomString(int length) {
			string str = "ABCDEFGHIJKLMNPQRSTUVWXYZ123456789";
			char[] chars = str.ToCharArray();
			StringBuilder builder = new StringBuilder();
			for (int i = 0; i < length; i++) {
				builder.Append(chars[rand.Next(str.Length - 1)]);
			}
			return builder.ToString();
		}

		public static Color GetRandomColor() {
			return Color.FromArgb(rand.Next(220), rand.Next(220), rand.Next(220));
		}

		public static MemoryStream GetValidImage(string code) {
			Bitmap bitmap = new Bitmap(60, 22);
			Graphics g = Graphics.FromImage(bitmap);
			g.Clear(Color.White);
			Font font = new Font("Comic Sans MS", 13.0f);
			SolidBrush brush = new SolidBrush(GetRandomColor());
			char[] codes = code.ToCharArray();
			int x = 0, y = 0;
			for (int i = 0; i < codes.Length; i++) {
				g.DrawString(codes[i].ToString(), font, new SolidBrush(GetRandomColor()), x, y);
				x += 13;
			}

			MemoryStream stream = new MemoryStream();
			bitmap.Save(stream, ImageFormat.Png);
			bitmap.Dispose();
			g.Dispose();
			return stream;
		}

		public static string MD5(string str) {
			byte[] bytes = Encoding.Default.GetBytes(str);
			bytes = new MD5CryptoServiceProvider().ComputeHash(bytes);
			string str2 = "";
			for (int i = 0; i < bytes.Length; i++) {
				str2 = str2 + bytes[i].ToString("x").PadLeft(2, '0');
			}
			return str2;
		}

		/// <summary>
		/// 传入List集合，得到将此集合批量插入数据的sql
		/// </summary>
		/// <typeparam name="T">要插入数据的类型</typeparam>
		/// <param name="list">数据集合</param>
		/// <param name="name">数据库中表的名字</param>
		/// <returns></returns>
		public static string GetSqlByInsert<T>(List<T> list, string name) {
			string sql = null;
			if (list.Count > 0) {
				PropertyInfo[] pros = list[0].GetType().GetProperties();
				string lie = null;
				for (int i = 0; i < pros.Length; i++) {
					lie += "," + pros[i].Name;
				}
				if (lie != null) {
					lie = lie.Substring(1);
				}
				foreach (var item in list) {
					string val = null;
					for (int i = 0; i < pros.Length; i++) {
						var type = pros[i].GetValue(item).GetType();
						if (type == typeof(double)) {
							val += "," + pros[i].GetValue(item);
						} else if (type == typeof(decimal)) {
							val += "," + pros[i].GetValue(item);
						} else if (type == typeof(int)) {
							val += "," + pros[i].GetValue(item);
						} else {
							val += "," + "'" + pros[i].GetValue(item) + "'";
						}
					}
					val = val.Substring(1);
					sql += "Insert into " + name + "(" + lie + ") values(" + val + ") ";
				}
			}
			return sql;
		}


		/// <summary>
		/// 引用实例深度克隆方法。
		/// </summary>
		/// <typeparam name="T">克隆实例类型</typeparam>
		/// <param name="serializableObject">可序列化的实例</param>
		/// <returns></returns>
		public static T CloneOf<T>(T serializableObject) {
			object copy = null;
			MemoryStream ms = new MemoryStream();
			BinaryFormatter bf = new BinaryFormatter();
			bf.Serialize(ms, serializableObject);
			ms.Position = 0;
			copy = (T)bf.Deserialize(ms);
			ms.Close();
			return (T)copy;
		}

		public static string SerializeObject(object value) {
			return JsonConvert.SerializeObject(value, new JsonSerializerSettings { ReferenceLoopHandling = ReferenceLoopHandling.Ignore });
		}

		public static T DeserializeObject<T>(string value) {
			return JsonConvert.DeserializeObject<T>(value, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore, MissingMemberHandling = MissingMemberHandling.Ignore });
		}

		/// <summary>
		/// 获取IP地址
		/// </summary>
		public static string IPAddress {
			get {
				string userHostAddress;
				HttpRequest request = HttpContext.Current.Request;
				if (string.IsNullOrEmpty(request.ServerVariables["HTTP_X_FORWARDED_FOR"])) {
					userHostAddress = request.ServerVariables["REMOTE_ADDR"];
				} else {
					userHostAddress = request.ServerVariables["HTTP_X_FORWARDED_FOR"];
				}
				if (string.IsNullOrEmpty(userHostAddress)) {
					userHostAddress = request.UserHostAddress;
				}
				return userHostAddress;
			}
		}
	}
}
