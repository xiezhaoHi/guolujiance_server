using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source {
	public class Menus {
		public Menus() {
			this.Child = new List<Menus>();
		}
		public System.Guid Id { get; set; }
		public string Name { get; set; }
		public string Url { get; set; }
		public string Icon { get; set; }
		public List<Menus> Child { get; set; }
	}
}
