USE [GasMonitor]
GO
/****** 对象:  Table [dbo].[SYS_Menus]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Menus](
	[Id] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Name] [nvarchar](30) NOT NULL,
	[ParentId] [uniqueidentifier] NULL,
	[Url] [nvarchar](150) NULL,
	[Icon] [nvarchar](50) NULL,
	[Sort] [int] NOT NULL,
	[IsEnabled] [bit] NOT NULL DEFAULT ((1)),
	[CreateUserId] [uniqueidentifier] NOT NULL,
	[CreateDate] [datetime] NOT NULL DEFAULT (getdate()),
	[ModifyUserId] [uniqueidentifier] NULL,
	[ModifyDate] [datetime] NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'Id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'显示名' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'Name'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'父编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'ParentId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'链接' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'Url'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'图标' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'Icon'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'排序' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'Sort'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否启用' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'IsEnabled'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建人编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'CreateUserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'CreateDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'修改人编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'ModifyUserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'修改日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus', @level2type=N'COLUMN',@level2name=N'ModifyDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'菜单表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Menus'
GO
/****** 对象:  Table [dbo].[SYS_Organization]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Organization](
	[Id] [uniqueidentifier] NOT NULL,
	[Name] [nvarchar](30) NOT NULL,
	[ParentId] [uniqueidentifier] NULL,
	[AreaCode] [nvarchar](20) NULL,
	[Remark] [nvarchar](200) NULL,
	[Level] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization', @level2type=N'COLUMN',@level2name=N'Id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'部门名称' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization', @level2type=N'COLUMN',@level2name=N'Name'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'上级部门' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization', @level2type=N'COLUMN',@level2name=N'ParentId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'地区编码' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization', @level2type=N'COLUMN',@level2name=N'AreaCode'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'备注' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization', @level2type=N'COLUMN',@level2name=N'Remark'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'排序' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization', @level2type=N'COLUMN',@level2name=N'Level'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'组织机构' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Organization'
GO
/****** 对象:  Table [dbo].[DeviceData_20160325]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160325](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[SYS_Roles]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Roles](
	[Id] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Name] [nvarchar](50) NOT NULL,
	[CreateUserId] [uniqueidentifier] NOT NULL,
	[CreateDate] [datetime] NOT NULL,
	[ModifyUserId] [uniqueidentifier] NULL,
	[ModifyDate] [datetime] NULL,
	[IsEnabled] [bit] NOT NULL,
	[Remark] [nvarchar](max) NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'Id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'名称' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'Name'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建人编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'CreateUserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'CreateDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'修改人编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'ModifyUserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'修改日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'ModifyDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否启用' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'IsEnabled'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'备注' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles', @level2type=N'COLUMN',@level2name=N'Remark'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Roles'
GO
/****** 对象:  Table [dbo].[DeviceData_20160328]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160328](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[DeviceData_20160329]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160329](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[sysdiagrams]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[sysdiagrams](
	[name] [sysname] NOT NULL,
	[principal_id] [int] NOT NULL,
	[diagram_id] [int] IDENTITY(1,1) NOT NULL,
	[version] [int] NULL,
	[definition] [varbinary](max) NULL,
PRIMARY KEY CLUSTERED 
(
	[diagram_id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY],
UNIQUE NONCLUSTERED 
(
	[principal_id] ASC,
	[name] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** 对象:  StoredProcedure [dbo].[DeviceData]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE proc [dbo].[DeviceData]
@DeviceId nvarchar(50), @Start NCHAR(8), @End NCHAR(8) --入参
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' AND name BETWEEN 'DeviceData_'+@Start  AND 'DeviceData_'+@End  ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)

OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
SET @sql='SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
SET @sql = @sql + ' WHERE DeviceId = '+''''+@DeviceId+''''

EXEC sp_executesql @sql
GO
/****** 对象:  Table [dbo].[DeviceData_20160331]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160331](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[DeviceData_20160330]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160330](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[DeviceData_20160401]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160401](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  StoredProcedure [dbo].[sp_upgraddiagrams]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_upgraddiagrams]
	AS
	BEGIN
		IF OBJECT_ID(N'dbo.sysdiagrams') IS NOT NULL
			return 0;
	
		CREATE TABLE dbo.sysdiagrams
		(
			name sysname NOT NULL,
			principal_id int NOT NULL,	-- we may change it to varbinary(85)
			diagram_id int PRIMARY KEY IDENTITY,
			version int,
	
			definition varbinary(max)
			CONSTRAINT UK_principal_name UNIQUE
			(
				principal_id,
				name
			)
		);


		/* Add this if we need to have some form of extended properties for diagrams */
		/*
		IF OBJECT_ID(N'dbo.sysdiagram_properties') IS NULL
		BEGIN
			CREATE TABLE dbo.sysdiagram_properties
			(
				diagram_id int,
				name sysname,
				value varbinary(max) NOT NULL
			)
		END
		*/

		IF OBJECT_ID(N'dbo.dtproperties') IS NOT NULL
		begin
			insert into dbo.sysdiagrams
			(
				[name],
				[principal_id],
				[version],
				[definition]
			)
			select	 
				convert(sysname, dgnm.[uvalue]),
				DATABASE_PRINCIPAL_ID(N'dbo'),			-- will change to the sid of sa
				0,							-- zero for old format, dgdef.[version],
				dgdef.[lvalue]
			from dbo.[dtproperties] dgnm
				inner join dbo.[dtproperties] dggd on dggd.[property] = 'DtgSchemaGUID' and dggd.[objectid] = dgnm.[objectid]	
				inner join dbo.[dtproperties] dgdef on dgdef.[property] = 'DtgSchemaDATA' and dgdef.[objectid] = dgnm.[objectid]
				
			where dgnm.[property] = 'DtgSchemaNAME' and dggd.[uvalue] like N'_EA3E6268-D998-11CE-9454-00AA00A3F36E_' 
			return 2;
		end
		return 1;
	END
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceData]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE  proc [dbo].[sql_DeviceData]
@str nvarchar(max) OUTPUT
as
DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)
DECLARE cursor_tbls CURSOR
FOR
 --SELECT name FROM sys.tables WHERE [type]='U' AND name BETWEEN 'Rig_DeviceData_20150917' AND 'Rig_DeviceData_20151001' ORDER BY name
 SELECT name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name
OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
SET @sql='SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
set @str = @sql
select @sql
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataById]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE proc [dbo].[sql_DeviceDataById]
@DeviceId nvarchar(50), --入参
@str nvarchar(max) output
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)
 
OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
set @sql = 'select top 1 A.*,B.Name AS DeviceName,B.Number, C.AreaName,C.Name as BoilerName from ('
SET @sql=@sql+'SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
SET @sql = @sql + ') as A  
	INNER JOIN dbo.Device AS B ON A.DeviceId=B.Id
	INNER JOIN dbo.Boiler AS C ON B.BoilerId=C.Id
	WHERE DeviceId = '+''''+@DeviceId+''''+' ORDER BY CurrentTime desc'

SET @str = @sql
GO
/****** 对象:  Table [dbo].[DeviceData_20160405]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160405](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataByIdAverage]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE proc [dbo].[sql_DeviceDataByIdAverage]
@DeviceId nvarchar(50), --入参
@str nvarchar(max) output
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)

OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
set @sql = 'select avg(FlameTemperature) as d1,avg(NegativePressure) as d2,avg(Data3) as d3,avg(Data4) as d4 from  (select top 90 * from ('
SET @sql=@sql+'SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
SET @sql = @sql + ') as tb1 WHERE DeviceId = '+''''+@DeviceId+''''+' ORDER BY CurrentTime desc) as tb2 group by DeviceId'

SET @str = @sql
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataByIdFirstXiangxi]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE proc [dbo].[sql_DeviceDataByIdFirstXiangxi]
@DeviceId nvarchar(50), --入参
@str nvarchar(max) output
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)

OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
set @sql = 'select top 390 * from ('
SET @sql=@sql+'SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
SET @sql = @sql + ') as tb1 WHERE DeviceId = '+''''+@DeviceId+''''+' ORDER BY CurrentTime desc'

SET @str = @sql
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataTop]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE proc [dbo].[sql_DeviceDataTop]
@str nvarchar(max) output
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)

OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
set @sql = 'select top 1 * from ('
SET @sql=@sql+'SELECT * FROM '+@tbl
print @sql
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
SET @sql = @sql + ') as tb1'+' ORDER BY CurrentTime desc'

SET @str = @sql
GO
/****** 对象:  UserDefinedFunction [dbo].[fn_diagramobjects]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[fn_diagramobjects]() 
	RETURNS int
	WITH EXECUTE AS N'dbo'
	AS
	BEGIN
		declare @id_upgraddiagrams		int
		declare @id_sysdiagrams			int
		declare @id_helpdiagrams		int
		declare @id_helpdiagramdefinition	int
		declare @id_creatediagram	int
		declare @id_renamediagram	int
		declare @id_alterdiagram 	int 
		declare @id_dropdiagram		int
		declare @InstalledObjects	int

		select @InstalledObjects = 0

		select 	@id_upgraddiagrams = object_id(N'dbo.sp_upgraddiagrams'),
			@id_sysdiagrams = object_id(N'dbo.sysdiagrams'),
			@id_helpdiagrams = object_id(N'dbo.sp_helpdiagrams'),
			@id_helpdiagramdefinition = object_id(N'dbo.sp_helpdiagramdefinition'),
			@id_creatediagram = object_id(N'dbo.sp_creatediagram'),
			@id_renamediagram = object_id(N'dbo.sp_renamediagram'),
			@id_alterdiagram = object_id(N'dbo.sp_alterdiagram'), 
			@id_dropdiagram = object_id(N'dbo.sp_dropdiagram')

		if @id_upgraddiagrams is not null
			select @InstalledObjects = @InstalledObjects + 1
		if @id_sysdiagrams is not null
			select @InstalledObjects = @InstalledObjects + 2
		if @id_helpdiagrams is not null
			select @InstalledObjects = @InstalledObjects + 4
		if @id_helpdiagramdefinition is not null
			select @InstalledObjects = @InstalledObjects + 8
		if @id_creatediagram is not null
			select @InstalledObjects = @InstalledObjects + 16
		if @id_renamediagram is not null
			select @InstalledObjects = @InstalledObjects + 32
		if @id_alterdiagram  is not null
			select @InstalledObjects = @InstalledObjects + 64
		if @id_dropdiagram is not null
			select @InstalledObjects = @InstalledObjects + 128
		
		return @InstalledObjects 
	END
GO
/****** 对象:  Table [dbo].[DeviceData_20160305]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_20160305](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[DeviceData_19000101]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeviceData_19000101](
	[DeviceId] [uniqueidentifier] NOT NULL,
	[CompanyName] [nvarchar](50) NOT NULL,
	[Address] [nvarchar](255) NOT NULL,
	[Longitude] [decimal](9, 6) NOT NULL,
	[Latitude] [decimal](9, 6) NOT NULL,
	[FlameTemperature] [float] NOT NULL,
	[NegativePressure] [float] NOT NULL,
	[Data3] [float] NOT NULL,
	[Data4] [float] NOT NULL,
	[CurrentTime] [datetime] NOT NULL,
	[BoilerId] [uniqueidentifier] NULL
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[BaseCode]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[BaseCode](
	[Id] [uniqueidentifier] NOT NULL,
	[Code] [nvarchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
 CONSTRAINT [PK_BaseCode] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** 对象:  Table [dbo].[Boiler]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Boiler](
	[Id] [uniqueidentifier] NOT NULL,
	[Longitude] [decimal](9, 6) NULL,
	[Latitude] [decimal](9, 6) NULL,
	[Name] [nvarchar](150) NOT NULL,
	[Code] [nvarchar](50) NULL,
	[AreaName] [nvarchar](50) NULL,
 CONSTRAINT [PK_Boiler] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** 对象:  StoredProcedure [dbo].[Pro_DevcieNewInfoByDevId]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[Pro_DevcieNewInfoByDevId]
	@deviceId UNIQUEIDENTIFIER
AS
BEGIN
DECLARE cursor_tbls CURSOR --使用游标
FOR
	SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name DESC

	DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)
	OPEN cursor_tbls
	FETCH NEXT FROM cursor_tabls INTO @tbl
	WHILE (@@FETCH_STATUS=0)
	
	BEGIN

	FETCH NEXT FROM cursor_tabls INTO @tbl

SET @sql=	'select top 1 * from (select * from '+ @tbl+' UNION ALL SELECT * FROM '+ @tbl +') as A
	INNER JOIN dbo.Device AS B ON A.DeviceId=B.Id
	INNER JOIN dbo.Boiler AS C ON B.BoilerId=C.Id
	WHERE DeviceId = @deviceId ORDER BY CurrentTime desc
	'
	--select top 1 * from(select * from @tbl) as A  
	--INNER JOIN dbo.Device AS B ON A.DeviceId=B.Id
	--INNER JOIN dbo.Boiler AS C ON B.BoilerId=C.Id
	--WHERE DeviceId = @deviceId ORDER BY CurrentTime desc
 END
	
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataHistorical]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE  proc [dbo].[sql_DeviceDataHistorical]
@id nvarchar(max),
@str nvarchar(max) output
as
DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)
DECLARE cursor_tbls CURSOR
FOR
 --SELECT name FROM sys.tables WHERE [type]='U' AND name BETWEEN 'Rig_DeviceData_20150917' AND 'Rig_DeviceData_20151001' ORDER BY name
 SELECT name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name
OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
SET @sql='select * from ( SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
set @sql = @sql+ ') as tb1 WHERE DeviceId = '+''''+@id+''''+' ORDER BY CurrentTime desc'
set @str = @sql
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataTable]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE proc [dbo].[sql_DeviceDataTable]
@TableName nvarchar(50) --入参
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50),@tbl_model NVARCHAR(50),@i int

set @i=0

OPEN cursor_tbls
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	if (@tbl=@TableName) begin
	set @i=1
	end
	set @tbl_model = @tbl
END
if (@i=0)
begin
set @sql =  'select * into '+@TableName+' from '+@tbl_model+' where 2=1' 
EXEC (@sql)
end
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
GO
/****** 对象:  StoredProcedure [dbo].[sql_DeviceDataByIdFirst]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create proc [dbo].[sql_DeviceDataByIdFirst]
@DeviceId nvarchar(50), --入参
@str nvarchar(max) output
as

DECLARE cursor_tbls CURSOR
FOR
 SELECT Name FROM sys.tables WHERE [type]='U' and name Like 'DeviceData_%' ORDER BY name desc

DECLARE @sql NVARCHAR(MAX), @tbl NVARCHAR(50)

OPEN cursor_tbls
FETCH NEXT FROM cursor_tbls INTO @tbl
set @sql = 'select top 120 * from ('
SET @sql=@sql+'SELECT * FROM '+@tbl
WHILE @@FETCH_STATUS=0
BEGIN
	FETCH NEXT FROM cursor_tbls INTO @tbl
	SET @sql = @sql + ' UNION ALL SELECT * FROM '+@tbl
END
CLOSE cursor_tbls
DEALLOCATE cursor_tbls
SET @sql = @sql + ') as tb1 WHERE DeviceId = '+''''+@DeviceId+''''+' ORDER BY CurrentTime desc'

SET @str = @sql
GO
/****** 对象:  Table [dbo].[Device]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Device](
	[Id] [uniqueidentifier] NOT NULL,
	[Number] [nvarchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[State] [int] NOT NULL,
	[Remark] [nvarchar](max) NULL,
	[IsDelete] [bit] NOT NULL DEFAULT ((0)),
	[CreateDate] [datetime] NOT NULL,
	[CreateUser] [uniqueidentifier] NOT NULL,
	[Longitude] [decimal](9, 6) NULL,
	[Latitude] [decimal](9, 6) NULL,
	[BoilerId] [uniqueidentifier] NULL,
	[Code] [nvarchar](50) NULL,
	[AreaName] [nvarchar](50) NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'Id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'Number'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'名字' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'Name'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'状态' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'State'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'备注' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'Remark'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否删除' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'IsDelete'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'CreateDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建人' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'CreateUser'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'锅炉ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Device', @level2type=N'COLUMN',@level2name=N'BoilerId'
GO
/****** 对象:  Table [dbo].[SYS_Role2Authority]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Role2Authority](
	[RoleId] [uniqueidentifier] NOT NULL,
	[AuthorityId] [uniqueidentifier] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[RoleId] ASC,
	[AuthorityId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Authority', @level2type=N'COLUMN',@level2name=N'RoleId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'功能编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Authority', @level2type=N'COLUMN',@level2name=N'AuthorityId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色功能关系表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Authority'
GO
/****** 对象:  Table [dbo].[SYS_Authorities]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Authorities](
	[Id] [uniqueidentifier] NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[Handler] [nvarchar](255) NOT NULL,
	[Action] [nvarchar](255) NOT NULL,
	[BtnClass] [nvarchar](50) NULL,
	[MenuId] [uniqueidentifier] NOT NULL,
	[CreateDate] [datetime] NOT NULL DEFAULT (getdate()),
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'Id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'名称' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'Name'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'控制器' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'Handler'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'行为' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'Action'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'按钮' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'BtnClass'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'菜单编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'MenuId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities', @level2type=N'COLUMN',@level2name=N'CreateDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'功能表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Authorities'
GO
/****** 对象:  Table [dbo].[SYS_Role2Menu]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Role2Menu](
	[RoleId] [uniqueidentifier] NOT NULL,
	[MenuId] [uniqueidentifier] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[RoleId] ASC,
	[MenuId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Menu', @level2type=N'COLUMN',@level2name=N'RoleId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'菜单编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Menu', @level2type=N'COLUMN',@level2name=N'MenuId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色菜单关系表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Menu'
GO
/****** 对象:  Table [dbo].[SYS_Users]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Users](
	[Id] [uniqueidentifier] NOT NULL,
	[Name] [nvarchar](50) NULL,
	[LoginName] [nvarchar](50) NOT NULL,
	[Password] [nvarchar](50) NOT NULL,
	[OrganizationId] [uniqueidentifier] NULL,
	[CreateUserId] [uniqueidentifier] NOT NULL,
	[CreateDate] [datetime] NOT NULL,
	[ModifyUserId] [uniqueidentifier] NULL,
	[ModifyDate] [datetime] NULL,
	[IsEnabled] [bit] NOT NULL,
	[Code] [nvarchar](50) NULL,
	[AreaName] [nvarchar](50) NULL,
	[Remark] [nvarchar](max) NULL,
 CONSTRAINT [PK__SYS_Users__2D27B809] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'Id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'显示名' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'Name'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'登录名' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'LoginName'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'密码' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'Password'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'组织编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'OrganizationId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建人编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'CreateUserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建日期' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'CreateDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'修改人编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'ModifyUserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'修改时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'ModifyDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否启用' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'IsEnabled'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'备注' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users', @level2type=N'COLUMN',@level2name=N'Remark'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'系统用户' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Users'
GO
/****** 对象:  Table [dbo].[SYS_Role2Permission]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_Role2Permission](
	[RoleId] [uniqueidentifier] NOT NULL,
	[PermissionId] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[RoleId] ASC,
	[PermissionId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Permission', @level2type=N'COLUMN',@level2name=N'RoleId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'权限编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Permission', @level2type=N'COLUMN',@level2name=N'PermissionId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色权限关系表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_Role2Permission'
GO
/****** 对象:  Table [dbo].[SYS_User2Role]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SYS_User2Role](
	[UserId] [uniqueidentifier] NOT NULL,
	[RoleId] [uniqueidentifier] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[UserId] ASC,
	[RoleId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用户编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_User2Role', @level2type=N'COLUMN',@level2name=N'UserId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_User2Role', @level2type=N'COLUMN',@level2name=N'RoleId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用户角色关系表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SYS_User2Role'
GO
/****** 对象:  Table [dbo].[AlarmRecord]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AlarmRecord](
	[DeviceId] [uniqueidentifier] NULL,
	[AlarmDataCeiling] [float] NULL,
	[AlarmDataFloor] [float] NULL,
	[AlarmDataType_English] [nvarchar](50) NULL,
	[AlarmDataType_Chinese] [nvarchar](50) NULL,
	[AlarmData] [float] NULL,
	[CurrentTime] [datetime] NULL,
	[AlarmTime] [datetime] NULL,
	[Id] [uniqueidentifier] NOT NULL,
 CONSTRAINT [PK__AlarmRecord__239E4DCF] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'设备ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'DeviceId'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报警数据的上限' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'AlarmDataCeiling'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报警数据的下限' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'AlarmDataFloor'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报警数据的类型' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'AlarmDataType_English'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报警数据值' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'AlarmData'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'数据采集时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'CurrentTime'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报警记录生成时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'AlarmTime'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AlarmRecord', @level2type=N'COLUMN',@level2name=N'Id'
GO
/****** 对象:  View [dbo].[ViewMenu]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE VIEW [dbo].[ViewMenu] AS 
SELECT   m1.Id, m1.Name, m2.Name AS ParentName, m1.ParentId, m1.Url, m1.Icon, m1.Sort, m1.IsEnabled, m1.CreateUserId, 
                m1.CreateDate, m1.ModifyUserId, m1.ModifyDate
FROM      dbo.SYS_Menus AS m1 LEFT OUTER JOIN
                dbo.SYS_Menus AS m2 ON m1.ParentId = m2.Id
GO
/****** 对象:  StoredProcedure [dbo].[sp_renamediagram]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_renamediagram]
	(
		@diagramname 		sysname,
		@owner_id		int	= null,
		@new_diagramname	sysname
	
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
		declare @theId 			int
		declare @IsDbo 			int
		
		declare @UIDFound 		int
		declare @DiagId			int
		declare @DiagIdTarg		int
		declare @u_name			sysname
		if((@diagramname is null) or (@new_diagramname is null))
		begin
			RAISERROR ('Invalid value', 16, 1);
			return -1
		end
	
		EXECUTE AS CALLER;
		select @theId = DATABASE_PRINCIPAL_ID();
		select @IsDbo = IS_MEMBER(N'db_owner'); 
		if(@owner_id is null)
			select @owner_id = @theId;
		REVERT;
	
		select @u_name = USER_NAME(@owner_id)
	
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname 
		if(@DiagId IS NULL or (@IsDbo = 0 and @UIDFound <> @theId))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1)
			return -3
		end
	
		-- if((@u_name is not null) and (@new_diagramname = @diagramname))	-- nothing will change
		--	return 0;
	
		if(@u_name is null)
			select @DiagIdTarg = diagram_id from dbo.sysdiagrams where principal_id = @theId and name = @new_diagramname
		else
			select @DiagIdTarg = diagram_id from dbo.sysdiagrams where principal_id = @owner_id and name = @new_diagramname
	
		if((@DiagIdTarg is not null) and  @DiagId <> @DiagIdTarg)
		begin
			RAISERROR ('The name is already used.', 16, 1);
			return -2
		end		
	
		if(@u_name is null)
			update dbo.sysdiagrams set [name] = @new_diagramname, principal_id = @theId where diagram_id = @DiagId
		else
			update dbo.sysdiagrams set [name] = @new_diagramname where diagram_id = @DiagId
		return 0
	END
GO
/****** 对象:  StoredProcedure [dbo].[sp_alterdiagram]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_alterdiagram]
	(
		@diagramname 	sysname,
		@owner_id	int	= null,
		@version 	int,
		@definition 	varbinary(max)
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
	
		declare @theId 			int
		declare @retval 		int
		declare @IsDbo 			int
		
		declare @UIDFound 		int
		declare @DiagId			int
		declare @ShouldChangeUID	int
	
		if(@diagramname is null)
		begin
			RAISERROR ('Invalid ARG', 16, 1)
			return -1
		end
	
		execute as caller;
		select @theId = DATABASE_PRINCIPAL_ID();	 
		select @IsDbo = IS_MEMBER(N'db_owner'); 
		if(@owner_id is null)
			select @owner_id = @theId;
		revert;
	
		select @ShouldChangeUID = 0
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname 
		
		if(@DiagId IS NULL or (@IsDbo = 0 and @theId <> @UIDFound))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1);
			return -3
		end
	
		if(@IsDbo <> 0)
		begin
			if(@UIDFound is null or USER_NAME(@UIDFound) is null) -- invalid principal_id
			begin
				select @ShouldChangeUID = 1 ;
			end
		end

		-- update dds data			
		update dbo.sysdiagrams set definition = @definition where diagram_id = @DiagId ;

		-- change owner
		if(@ShouldChangeUID = 1)
			update dbo.sysdiagrams set principal_id = @theId where diagram_id = @DiagId ;

		-- update dds version
		if(@version is not null)
			update dbo.sysdiagrams set version = @version where diagram_id = @DiagId ;

		return 0
	END
GO
/****** 对象:  StoredProcedure [dbo].[sp_creatediagram]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_creatediagram]
	(
		@diagramname 	sysname,
		@owner_id		int	= null, 	
		@version 		int,
		@definition 	varbinary(max)
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
	
		declare @theId int
		declare @retval int
		declare @IsDbo	int
		declare @userName sysname
		if(@version is null or @diagramname is null)
		begin
			RAISERROR (N'E_INVALIDARG', 16, 1);
			return -1
		end
	
		execute as caller;
		select @theId = DATABASE_PRINCIPAL_ID(); 
		select @IsDbo = IS_MEMBER(N'db_owner');
		revert; 
		
		if @owner_id is null
		begin
			select @owner_id = @theId;
		end
		else
		begin
			if @theId <> @owner_id
			begin
				if @IsDbo = 0
				begin
					RAISERROR (N'E_INVALIDARG', 16, 1);
					return -1
				end
				select @theId = @owner_id
			end
		end
		-- next 2 line only for test, will be removed after define name unique
		if EXISTS(select diagram_id from dbo.sysdiagrams where principal_id = @theId and name = @diagramname)
		begin
			RAISERROR ('The name is already used.', 16, 1);
			return -2
		end
	
		insert into dbo.sysdiagrams(name, principal_id , version, definition)
				VALUES(@diagramname, @theId, @version, @definition) ;
		
		select @retval = @@IDENTITY 
		return @retval
	END
GO
/****** 对象:  StoredProcedure [dbo].[sp_dropdiagram]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_dropdiagram]
	(
		@diagramname 	sysname,
		@owner_id	int	= null
	)
	WITH EXECUTE AS 'dbo'
	AS
	BEGIN
		set nocount on
		declare @theId 			int
		declare @IsDbo 			int
		
		declare @UIDFound 		int
		declare @DiagId			int
	
		if(@diagramname is null)
		begin
			RAISERROR ('Invalid value', 16, 1);
			return -1
		end
	
		EXECUTE AS CALLER;
		select @theId = DATABASE_PRINCIPAL_ID();
		select @IsDbo = IS_MEMBER(N'db_owner'); 
		if(@owner_id is null)
			select @owner_id = @theId;
		REVERT; 
		
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname 
		if(@DiagId IS NULL or (@IsDbo = 0 and @UIDFound <> @theId))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1)
			return -3
		end
	
		delete from dbo.sysdiagrams where diagram_id = @DiagId;
	
		return 0;
	END
GO
/****** 对象:  StoredProcedure [dbo].[sp_helpdiagramdefinition]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_helpdiagramdefinition]
	(
		@diagramname 	sysname,
		@owner_id	int	= null 		
	)
	WITH EXECUTE AS N'dbo'
	AS
	BEGIN
		set nocount on

		declare @theId 		int
		declare @IsDbo 		int
		declare @DiagId		int
		declare @UIDFound	int
	
		if(@diagramname is null)
		begin
			RAISERROR (N'E_INVALIDARG', 16, 1);
			return -1
		end
	
		execute as caller;
		select @theId = DATABASE_PRINCIPAL_ID();
		select @IsDbo = IS_MEMBER(N'db_owner');
		if(@owner_id is null)
			select @owner_id = @theId;
		revert; 
	
		select @DiagId = diagram_id, @UIDFound = principal_id from dbo.sysdiagrams where principal_id = @owner_id and name = @diagramname;
		if(@DiagId IS NULL or (@IsDbo = 0 and @UIDFound <> @theId ))
		begin
			RAISERROR ('Diagram does not exist or you do not have permission.', 16, 1);
			return -3
		end

		select version, definition FROM dbo.sysdiagrams where diagram_id = @DiagId ; 
		return 0
	END
GO
/****** 对象:  StoredProcedure [dbo].[sp_helpdiagrams]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_helpdiagrams]
	(
		@diagramname sysname = NULL,
		@owner_id int = NULL
	)
	WITH EXECUTE AS N'dbo'
	AS
	BEGIN
		DECLARE @user sysname
		DECLARE @dboLogin bit
		EXECUTE AS CALLER;
			SET @user = USER_NAME();
			SET @dboLogin = CONVERT(bit,IS_MEMBER('db_owner'));
		REVERT;
		SELECT
			[Database] = DB_NAME(),
			[Name] = name,
			[ID] = diagram_id,
			[Owner] = USER_NAME(principal_id),
			[OwnerID] = principal_id
		FROM
			sysdiagrams
		WHERE
			(@dboLogin = 1 OR USER_NAME(principal_id) = @user) AND
			(@diagramname IS NULL OR name = @diagramname) AND
			(@owner_id IS NULL OR principal_id = @owner_id)
		ORDER BY
			4, 5, 1
	END
GO
/****** 对象:  View [dbo].[ViewDeviceAlarm]    脚本日期: 01/05/2017 10:24:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE VIEW [dbo].[ViewDeviceAlarm]
AS
SELECT   dbo.Device.Name, dbo.Device.Number, dbo.AlarmRecord.DeviceId, dbo.AlarmRecord.AlarmDataCeiling, 
                dbo.AlarmRecord.AlarmDataFloor, dbo.AlarmRecord.AlarmDataType_English, dbo.AlarmRecord.AlarmDataType_Chinese, 
                dbo.AlarmRecord.AlarmData, dbo.AlarmRecord.CurrentTime, dbo.AlarmRecord.AlarmTime, dbo.AlarmRecord.Id
FROM      dbo.AlarmRecord INNER JOIN
                dbo.Device ON dbo.AlarmRecord.DeviceId = dbo.Device.Id
GO
EXEC sys.sp_addextendedproperty @name=N'MS_DiagramPane1', @value=N'[0E232FF0-B466-11cf-A24F-00AA00A3EFFF, 1.00]
Begin DesignProperties = 
   Begin PaneConfigurations = 
      Begin PaneConfiguration = 0
         NumPanes = 4
         Configuration = "(H (1[13] 4[48] 2[20] 3) )"
      End
      Begin PaneConfiguration = 1
         NumPanes = 3
         Configuration = "(H (1 [50] 4 [25] 3))"
      End
      Begin PaneConfiguration = 2
         NumPanes = 3
         Configuration = "(H (1 [50] 2 [25] 3))"
      End
      Begin PaneConfiguration = 3
         NumPanes = 3
         Configuration = "(H (4 [30] 2 [40] 3))"
      End
      Begin PaneConfiguration = 4
         NumPanes = 2
         Configuration = "(H (1 [56] 3))"
      End
      Begin PaneConfiguration = 5
         NumPanes = 2
         Configuration = "(H (2 [66] 3))"
      End
      Begin PaneConfiguration = 6
         NumPanes = 2
         Configuration = "(H (4 [50] 3))"
      End
      Begin PaneConfiguration = 7
         NumPanes = 1
         Configuration = "(V (3))"
      End
      Begin PaneConfiguration = 8
         NumPanes = 3
         Configuration = "(H (1[56] 4[18] 2) )"
      End
      Begin PaneConfiguration = 9
         NumPanes = 2
         Configuration = "(H (1 [75] 4))"
      End
      Begin PaneConfiguration = 10
         NumPanes = 2
         Configuration = "(H (1[66] 2) )"
      End
      Begin PaneConfiguration = 11
         NumPanes = 2
         Configuration = "(H (4 [60] 2))"
      End
      Begin PaneConfiguration = 12
         NumPanes = 1
         Configuration = "(H (1) )"
      End
      Begin PaneConfiguration = 13
         NumPanes = 1
         Configuration = "(V (4))"
      End
      Begin PaneConfiguration = 14
         NumPanes = 1
         Configuration = "(V (2))"
      End
      ActivePaneConfig = 0
   End
   Begin DiagramPane = 
      Begin Origin = 
         Top = 0
         Left = 0
      End
      Begin Tables = 
         Begin Table = "AlarmRecord"
            Begin Extent = 
               Top = 6
               Left = 38
               Bottom = 145
               Right = 266
            End
            DisplayFlags = 280
            TopColumn = 0
         End
         Begin Table = "Device"
            Begin Extent = 
               Top = 6
               Left = 304
               Bottom = 145
               Right = 458
            End
            DisplayFlags = 280
            TopColumn = 0
         End
      End
   End
   Begin SQLPane = 
   End
   Begin DataPane = 
      Begin ParameterDefaults = ""
      End
   End
   Begin CriteriaPane = 
      Begin ColumnWidths = 11
         Column = 3780
         Alias = 900
         Table = 1170
         Output = 720
         Append = 1400
         NewValue = 1170
         SortType = 1350
         SortOrder = 1410
         GroupBy = 1350
         Filter = 1350
         Or = 1350
         Or = 1350
         Or = 1350
      End
   End
End
' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'VIEW',@level1name=N'ViewDeviceAlarm'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_DiagramPaneCount', @value=1 , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'VIEW',@level1name=N'ViewDeviceAlarm'
GO
/****** 对象:  ForeignKey [FK__AlarmReco__Devic__300424B4]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[AlarmRecord]  WITH CHECK ADD  CONSTRAINT [FK__AlarmReco__Devic__300424B4] FOREIGN KEY([DeviceId])
REFERENCES [dbo].[Device] ([Id])
GO
ALTER TABLE [dbo].[AlarmRecord] CHECK CONSTRAINT [FK__AlarmReco__Devic__300424B4]
GO
/****** 对象:  ForeignKey [FK__SYS_Autho__MenuI__30F848ED]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Authorities]  WITH CHECK ADD FOREIGN KEY([MenuId])
REFERENCES [dbo].[SYS_Menus] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
/****** 对象:  ForeignKey [FK__SYS_Role2__Autho__31EC6D26]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Role2Authority]  WITH CHECK ADD FOREIGN KEY([AuthorityId])
REFERENCES [dbo].[SYS_Authorities] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
/****** 对象:  ForeignKey [FK__SYS_Role2__RoleI__32E0915F]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Role2Authority]  WITH CHECK ADD FOREIGN KEY([RoleId])
REFERENCES [dbo].[SYS_Roles] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
/****** 对象:  ForeignKey [FK__SYS_Role2__MenuI__33D4B598]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Role2Menu]  WITH CHECK ADD FOREIGN KEY([MenuId])
REFERENCES [dbo].[SYS_Menus] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
/****** 对象:  ForeignKey [FK__SYS_Role2__RoleI__34C8D9D1]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Role2Menu]  WITH CHECK ADD FOREIGN KEY([RoleId])
REFERENCES [dbo].[SYS_Roles] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
/****** 对象:  ForeignKey [FK__SYS_Role2__RoleI__35BCFE0A]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Role2Permission]  WITH CHECK ADD FOREIGN KEY([RoleId])
REFERENCES [dbo].[SYS_Roles] ([Id])
GO
/****** 对象:  ForeignKey [FK__SYS_User2__RoleI__36B12243]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_User2Role]  WITH CHECK ADD FOREIGN KEY([RoleId])
REFERENCES [dbo].[SYS_Roles] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
/****** 对象:  ForeignKey [FK__SYS_User2__UserI__37A5467C]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_User2Role]  WITH CHECK ADD  CONSTRAINT [FK__SYS_User2__UserI__37A5467C] FOREIGN KEY([UserId])
REFERENCES [dbo].[SYS_Users] ([Id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[SYS_User2Role] CHECK CONSTRAINT [FK__SYS_User2__UserI__37A5467C]
GO
/****** 对象:  ForeignKey [FK__SYS_Users__Organ__38996AB5]    脚本日期: 01/05/2017 10:24:04 ******/
ALTER TABLE [dbo].[SYS_Users]  WITH CHECK ADD  CONSTRAINT [FK__SYS_Users__Organ__38996AB5] FOREIGN KEY([OrganizationId])
REFERENCES [dbo].[SYS_Organization] ([Id])
GO
ALTER TABLE [dbo].[SYS_Users] CHECK CONSTRAINT [FK__SYS_Users__Organ__38996AB5]
GO
