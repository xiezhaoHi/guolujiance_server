using System;
using System.Collections;
using System.Text.RegularExpressions;
using System.Web;
using System.Web.Caching;

namespace RTMC.Core {
    /// <summary>
    /// RT缓存类
    /// </summary>
    public sealed class RTCache {
        private static Cache _cache;//web应用程序缓存
        public const int DayFactor = 0x4380;
        private static int Factor = 5;
        public const int HourFactor = 720;
        public const int MinuteFactor = 12;
        public const double SecondFactor = 0.2;

        private RTCache() { }

        static RTCache() {
            HttpContext current = HttpContext.Current;
            if (current != null) {
                _cache = current.Cache;
            } else {
                _cache = HttpRuntime.Cache;
            }
        }

        public static void Clear() {
            IDictionaryEnumerator enumerator = _cache.GetEnumerator();
            ArrayList list = new ArrayList();
            while (enumerator.MoveNext()) {
                list.Add(enumerator.Key);
            }
            foreach (string str in list) {
                _cache.Remove(str);
            }
        }


        public static void ClearMenu() {
            IDictionaryEnumerator enumerator = _cache.GetEnumerator();
            ArrayList list = new ArrayList();
            while (enumerator.MoveNext()) {
                if (enumerator.Key.ToString().IndexOf("Menus_") >= 0) {
                    list.Add(enumerator.Key);
                }
            }
            foreach (string str in list) {
                _cache.Remove(str);
            }
        }

        public static object Get(string key) {
            return _cache[key];
        }

        public static void Insert(string key, object obj) {
            Insert(key, obj, null, 1);
        }

        public static void Insert(string key, object obj, int seconds) {
            Insert(key, obj, null, seconds);
        }

        public static void Insert(string key, object obj, CacheDependency dep) {
            Insert(key, obj, dep, 0x21c0);
        }

        public static void Insert(string key, object obj, int seconds, CacheItemPriority priority) {
            Insert(key, obj, null, seconds, priority);
        }

        public static void Insert(string key, object obj, CacheDependency dep, int seconds) {
            Insert(key, obj, dep, seconds, CacheItemPriority.Normal);
        }

        public static void Insert(string key, object obj, CacheDependency dep, int seconds, CacheItemPriority priority) {
            if (obj != null) {
                _cache.Insert(key, obj, dep, DateTime.Now.AddSeconds((double)(Factor * seconds)), TimeSpan.Zero, priority, null);
            }
        }

        public static void Max(string key, object obj) {
            Max(key, obj, null);
        }

        public static void Max(string key, object obj, CacheDependency dep) {
            if (obj != null) {
                _cache.Insert(key, obj, dep, DateTime.MaxValue, TimeSpan.Zero, CacheItemPriority.AboveNormal, null);
            }
        }

        public static void MicroInsert(string key, object obj, int secondFactor) {
            if (obj != null) {
                _cache.Insert(key, obj, null, DateTime.Now.AddSeconds((double)(Factor * secondFactor)), TimeSpan.Zero);
            }
        }

        public static void Remove(string key) {
            _cache.Remove(key);
        }

        /// <summary>
        /// 移除模式
        /// </summary>
        /// <param name="pattern"></param>
        public static void RemoveByPattern(string pattern) {
            IDictionaryEnumerator enumerator = _cache.GetEnumerator();
            Regex regex = new Regex(pattern, RegexOptions.Singleline | RegexOptions.Compiled | RegexOptions.IgnoreCase);
            while (enumerator.MoveNext()) {
                if (regex.IsMatch(enumerator.Key.ToString())) {
                    _cache.Remove(enumerator.Key.ToString());
                }
            }
        }

        /// <summary>
        /// 复位
        /// </summary>
        /// <param name="cacheFactor"></param>
        public static void ReSetFactor(int cacheFactor) {
            Factor = cacheFactor;
        }

        /// <summary>
        /// 第二种原因
        /// </summary>
        /// <param name="seconds"></param>
        /// <returns></returns>
        public static int SecondFactorCalculate(int seconds) {
            return Convert.ToInt32(Math.Round((double)(seconds * 0.2)));
        }
    }
}
