using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source.Model {
	public class ViewDeviceData {
		public Guid DeviceId { get; set; }
		public string CompanyName { get; set; }
		public string Address { get; set; }
		public decimal Longitude { get; set; }
		public decimal Latitude { get; set; }
		public long SulfurDioxide { get; set; }
		public long NitricOxide { get; set; }
		public long CarbonMonoxide { get; set; }
		public long Hydrogen { get; set; }
		public decimal Oxygen { get; set; }
		public decimal CarbonDioxide { get; set; }
		public DateTime CurrentTime { get; set; }
		public string Number { get; set; }
		public Guid? BoilerId { get; set; }
		public string Name { get; set; }
	}
}
