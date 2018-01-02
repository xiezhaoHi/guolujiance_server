using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source {
	public class BaseCodeTreeModel {
		[DataMember]
		public string Code { get; set; }
		[DataMember]
		public string Name { get; set; }
		[DataMember]
		public string ParentCode { get; set; }
		[DataMember]
		public IEnumerable<BaseCodeTreeModel> Childs { get; set; }
	}
}
