using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source.Model {
	public class ThresholdData {
		public Guid Id { get; set; }
		public long SulfurDioxideAlertLineCeiling { get; set; }
		public long SulfurDioxideAlertLineFloor { get; set; }
		public long NitricOxideAlertLineCeiling { get; set; }
		public long NitricOxideAlertLineFloor { get; set; }
		public long CarbonMonoxideAlertLineCeiling { get; set; }
		public long CarbonMonoxideAlertLineFloor { get; set; }
		public long HydrogenAlertLineCeiling { get; set; }
		public long HydrogenAlertLineFloor { get; set; }
		public decimal OxygenAlertLineCeiling { get; set; }
		public decimal OxygenAlertLineFloor { get; set; }
		public decimal CarbonDioxideAlertLineCeiling { get; set; }
		public decimal CarbonDioxideAlertLineFloor { get; set; }
	}
}
