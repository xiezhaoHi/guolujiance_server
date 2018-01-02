using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source.Model {
	public class Point {
		public Point() {

		}
		public Point(decimal Lng, decimal Lat) {
			this.lng = Lng;
			this.lat = Lat;
		}
		public decimal lng { get; set; }
		public decimal lat { get; set; }
	}
}
