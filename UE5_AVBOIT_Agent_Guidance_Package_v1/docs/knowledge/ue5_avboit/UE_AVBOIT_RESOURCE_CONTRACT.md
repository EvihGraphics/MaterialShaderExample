# UE AVBOIT Resource Contract

每个中间资源必须记录：

| 字段 | 内容 |
|---|---|
| Name | 资源名称 |
| Semantic | Occupancy / Extinction / Transmittance / Radiance |
| RDG Type | Texture / Buffer |
| Format | 精确格式 |
| Extent | ViewRect / full extent / 3D depth |
| Clear Value | 每帧初始值 |
| Writer | 写入 Pass |
| Reader | 读取 Pass |
| Value Range | 合法值域 |
| Precision Risk | 量化和溢出 |
| Lifetime | 单帧 / 历史 |
| Debug View | 对应调试模式 |

第一版禁止在没有文档的情况下复用同一资源承载多个不同语义。
