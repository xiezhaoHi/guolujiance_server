/// <reference path="/Scripts/utils/rtmc-util.js" />
var RTMC = {
    SYS_Menus: {
        Model: function () {
            this.Id = Util.guid();
            this.Name = null;
            this.ParentId = null;
            this.Url = null;
            this.Icon = null;
            this.Sort = null;
            this.IsEnabled = null;
            this.CreateUserId = null;
            this.CreateDate = null;
            this.ModifyUserId = null;
            this.ModifyDate = null;
        }
    },
    SYS_Users: {
        Model: function () {
            this.Id = Util.guid();
            this.LoginName = null;
            this.Password = null;
            this.Name = null;
            this.Age = null;
            this.Gender = null;
            this.OrganizationId = null;
            this.Duty = null;
            this.MobilePhone = null;
            this.Telephone = null;
            this.Email = null;
            this.CreateUserId = null;
            this.CreateDate = null;
            this.ModifyUserId = null;
            this.ModifyDate = null;
            this.IsEnabled = true;
            this.Remark = null;
            this.Code = null;
            this.AreaName = null;
        }
    },
    SYS_Roles: {
        Model: function () {
            this.Id = Util.guid();
            this.Name = null;
            this.CreateUserId = null;
            this.CreateDate = null;
            this.ModifyUserId = null;
            this.ModifyDate = null;
            this.IsEnabled = null;
            this.Remark = null;
        }
    },
    SYS_Authorities: {
        Model: function () {
            this.Id = Util.guid();
            this.Name = null;
            this.Handler = null;
            this.Action = null;
            this.BtnClass = null;
            this.MenuId = null;
            this.CreateDate = null;
        }
    },
    Device: {
        Model: function () {
            this.Id = Util.guid();
            this.Number = null;
            this.Name = null;
            this.State = null;
            this.IsDelete = null;
            this.Remark = null;
            this.CreateDate = null;
            this.CreateUser = null;
            this.Longitude = null;
            this.Latitude = null;
            this.Code = null;
            this.AreaName = null;
        }
    },
    DeviceData: {
    	Model: function () {
    		this.DeviceId = Util.guid();
    		this.CompanyName = null;
    		this.Address = null;
    		this.Longitude = null;
    		this.Latitude = null;
    		this.FlameTemperature = null;
    		this.NegativePressure = null;
    		this.Data3 = null;
    		this.Data4 = null;
    		this.CurrentTime = null;
    		this.BoilerId = null;
    	}
    },
    ViewDeviceData: {
    	Model: function () {
    		this.DeviceId = Util.guid();
    		this.CompanyName = null;
    		this.Address = null;
    		this.Longitude = null;
    		this.Latitude = null;
    		this.FlameTemperature = null;
    		this.NegativePressure = null;
    		this.Data3 = null;
    		this.Data4 = null;
    		this.CurrentTime = null;
    		this.Number = null;
    		this.Name = null;
    		this.BoilerId = null;
    	}
    },
    DeviceAlarmRecord:{
    	Model: function () {
    		this.Id = Util.guid();
    		this.AlarmDataCeiling = null;
    		this.AlarmDataFloor = null;
    		this.AlarmDataType_English = null;
    		this.AlarmDataType_Chinese = null;
    		this.AlarmData = null;
    		this.CurrentTime = null;
    		this.AlarmTime = null;
    		this.DeviceId = null;
    		this.Name = null;
    		this.Number = null;
    	}
    },
    ThresholdData: {
    	Model: function () {
    		this.Id = null;
    		this.FlameTemperatureAlertLineCeiling = null;
    		this.FlameTemperatureAlertLineFloor = null;
    		this.NegativePressureAlertLineCeiling = null;
    		this.NegativePressureAlertLineFloor = null;
    		this.Data3AlertLineCeiling = null;
    		this.Data3AlertLineFloor = null;
    		this.Data4AlertLineCeiling = null;
    		this.Data4AlertLineFloor = null;
    	}
    },
    Boiler: {
    	Model: function () {
    		this.Id = null;
    		this.Longitude = null;
    		this.Latitude = null;
    		this.Name = null;
    		this.Code = null;
    		this.AreaName = null;
    	}
    },
    EntityState: {
        UNCHANGED: 2,
        ADDED: 4,
        DELETED: 8,
        MODIFIED: 16
    }
};