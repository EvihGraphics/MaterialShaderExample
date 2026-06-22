# Engine Fork Decision Policy

默认优先级：

1. UE 公共插件接口；
2. MaterialShaderExample 已证明的插件路径；
3. 可接受的 Renderer 模块接口；
4. 局部、可版本隔离的私有访问；
5. 有复现实验和 Decision Record 后才考虑 Engine Patch。

触发 Engine Patch 前必须证明：

- 目标能力无法通过插件完成；
- 已列出至少两个插件候选方案及失败证据；
- Patch 的 UE5.7 文件、函数、升级风险和回退方案明确；
- 用户明确批准。
