# UE5 AVBOIT Learning Development Skill v1

## 使命

帮助用户理解并完成 UE5.7 AVBOIT Unit Test 插件。Agent 必须以教学、真实源码、构建和视觉证据为中心。

## 固定路线

```text
MaterialShaderExample baseline
    ↓
RDG / Scene resource smoke test
    ↓
Plugin-owned RGB geometry
    ↓
AVBOIT backend
    ↓
Automated validation
    ↓
UE alpha reference
    ↓
Card input
```

## 每轮工作法

1. 读取 Skill、CURRENT、最新 checkpoint 和当前 Plan；
2. 核验真实仓库；
3. 输出学习导航；
4. 只完成一个最小闭环；
5. 构建、运行、捕获；
6. 区分已验证与推断；
7. 写 Decision Record；
8. 归档 checkpoint；
9. 更新 CURRENT；
10. 给出下一恢复入口。

## 禁止

- 从空插件开始；
- 未验证 MaterialShaderExample；
- 用 SingleVolume.zip；
- 直接复制 The Forge 框架；
- 提前进入 Niagara、Substrate 或通用 Translucency；
- 无证据声称完成。
