using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source.Model {
	public class DeviceData {

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
		public Guid? BoilerId { get; set; }

		public string DeviceName { get; set; }
		public string Number { get; set; }
		public string BoilerName { get; set; }
		public string AreaName { get; set; }

		public TimeSpan TimeStamp {
			get {
				return CurrentTime - new DateTime(1970, 1, 1);
			}
			set {
				CurrentTime = new DateTime(1970, 1, 1) + value;
			}
		}
		public long UnixTime {
			get { return (long)TimeStamp.TotalMilliseconds; }
		}
		public decimal O2Min { get; set; }
		public decimal O2Max { get; set; }
		public decimal CO2Min { get; set; }
		public decimal CO2Max { get; set; }
		public long SO2Min { get; set; }
		public long SO2Max { get; set; }
		public long NOMin { get; set; }
		public long NOMax { get; set; }
		public long COMin { get; set; }
		public long COMax { get; set; }
		public long NOxMin { get; set; }
		public long NOxMax { get; set; }
	}

	public class TxtData {
		public DateTime CurrentTime { get; set; }

		public long SulfurDioxide { get; set; }
		public long NitricOxide { get; set; }
		public long CarbonMonoxide { get; set; }
		public long Hydrogen { get; set; }

		public decimal Oxygen { get; set; }
		public decimal CarbonDioxide { get; set; }
	}
}
