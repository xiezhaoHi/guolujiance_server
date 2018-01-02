CREATE TABLE [dbo].[Device] (
    [Id]         UNIQUEIDENTIFIER NOT NULL,
    [Number]     NVARCHAR (50)    NOT NULL,
    [Name]       NVARCHAR (50)    NOT NULL,
    [State]      INT              NOT NULL,
    [Remark]     NVARCHAR (MAX)   NULL,
    [IsDelete]   BIT              DEFAULT ((0)) NOT NULL,
    [CreateDate] DATETIME         NOT NULL,
    [CreateUser] UNIQUEIDENTIFIER NOT NULL,
    [Longitude]  DECIMAL (9, 6)   NULL,
    [Latitude]   DECIMAL (9, 6)   NULL,
    [BoilerId]   UNIQUEIDENTIFIER NULL,
    [Code]       NVARCHAR (50)    NULL,
    [AreaName]   NVARCHAR (50)    NULL,
    PRIMARY KEY CLUSTERED ([Id] ASC)
);


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'ID', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'Id';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'编号', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'Number';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'名字', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'Name';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'状态', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'State';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'备注', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'Remark';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'是否删除', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'IsDelete';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'创建日期', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'CreateDate';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'创建人', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'CreateUser';


GO
EXECUTE sp_addextendedproperty @name = N'MS_Description', @value = N'锅炉ID', @level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Device', @level2type = N'COLUMN', @level2name = N'BoilerId';

