CREATE TABLE [dbo].[Boiler] (
    [Id]        UNIQUEIDENTIFIER NOT NULL,
    [Longitude] DECIMAL (9, 6)   NULL,
    [Latitude]  DECIMAL (9, 6)   NULL,
    [Name]      NVARCHAR (150)   NOT NULL,
    [Code]      NVARCHAR (50)    NULL,
    [AreaName]  NVARCHAR (50)    NULL,
    CONSTRAINT [PK_Boiler] PRIMARY KEY CLUSTERED ([Id] ASC)
);

