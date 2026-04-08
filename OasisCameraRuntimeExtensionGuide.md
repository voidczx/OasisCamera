<!-- From Penguin Assistant Start -->

# OasisCamera Runtime Extension Guide

如果你正在使用 OasisCamera 插件，并且想要在游戏运行期间动态地改变相机的行为（比如：角色从步行变成驾驶，或者进入特定区域后需要拉大 FOV），你可以适用插件提供的如下两种能力：

1. **Change Proxy State**（切换代理状态）
2. **Add Dynamic Setting**（添加动态配置）

这两种能力虽然都作用于当前的顶层相机模式（`TopMode`），但它们的分工完全不同。简单来说：前者负责“换人做事”，后者负责“给做事的人换份说明书”。

下文将详细说明它们的职责、差异和适用场景。

---

## 综述

在 OasisCamera 的设计里，`CameraMode` 主要由两个维度构成：

1. **执行流（Proxy Classes）**：这是一组按顺序执行的计算节点（Proxy），决定了相机“具体要做哪些运算”。
2. **配置（Instance Mode Settings）**：这是各类参数（Setting）和它们对应的运行期状态（RuntimeData），决定了相机“运算时用什么数值”。

在此基础上，这两种扩展方式的实质就很清晰了：**运行时扩展，本质上就是在这两个维度里选一个进行替换或修改**。

---

## 一、Change Proxy State：更换执行流

### 它是怎么工作的？

常用的对外接口在 `UOasisCameraComponentBase` 中：

```cpp
ChangeTopModeProxyState(FName InStateName);
```

当你在代码或蓝图中调用这个接口时，相机系统可不是简单地“改几个参数”，它做的是**全盘替换当前的执行链**。
底层的真实逻辑为：

1. **停止旧Proxy**：先对当前所有正在激活中的 Proxy 喊一声“停”，调用它们的 `Deactivate` 方法。
2. **清理执行流**：把当前帧要执行的 Proxy 列表清空。
3. **更换新执行流**：根据你传入的 `StateName`，找到对应的新 Proxy 列表（传入 `NAME_None` 会退回 Default 默认状态）。
4. **激活新Proxy**：把新 Proxy 挂载上去，并对它们调用 `Activate`。

*(注：系统会复用缓存的 Proxy 实例，不一定会频繁构造新对象，但生命周期方法 `Activate/Deactivate` 是一定会触发的。)*

### 适合什么场景？

当你发现**相机运算的流程和步骤本身需要发生改变**时，比如：

- 角色从“正常第三人称视角”切换到了“锁定目标视角”（此时需要引入全新的 `LookAt` 运算步骤）。
- 从“步行”变成了“驾驶载具”（整套相机跟随和相机防穿逻辑都不一样了）。
- 玩家开始攀爬、滑索或者进入某些对于相机表现而言比较特殊的，与通常相机行为有明显增删的交互状态。

### 可能的误用点

如果你仅仅是想：

- 把镜头的拉远一点（改一下 `SpringArm` 的长度）。
- 调整相机的跟随平滑度。
- 换一个 `LookAt` 的目标点。

**不建议适用 Change Proxy State** 这样做不仅浪费性能，原有的插值状态也会因为 Proxy 的重启而出现生硬的跳变。

---

## 二、Add Dynamic Setting：仅修改执行流所参考的配置

### 它是怎么工作的？

入口同样在 `UOasisCameraComponentBase` 中：

```cpp
TryAddTopModeDynamicSetting(UOasisCameraSettingBase* InDynamicSetting);
TryRemoveTopModeDynamicSetting(FName SettingTypeName);
```

如果说 `Change Proxy State` 是“换人”，那 `Add Dynamic Setting` 就是“给现有的工人换份说明书”。
在 `UOasisCameraModeBase` 里，每个配置类型（SettingType）的槽位都有个非常直接的优先级决议规则：
**DynamicSetting 优先 > 如果没有的话再退回使用 StaticSetting**。

所以，添加动态配置的本质是**修改当前 Mode 中某个特定配置类型的有效数据来源**。

- 如果这个配置本来就有，那就是**运行时覆盖**。
- 如果这个配置原本没有，那就是**凭空拓展**出一条新的配置槽位。

### RuntimeData 在 DynamicSetting 变换时的表现

RuntimeData 是附属于 CameraSetting 的，用于维护可能在 Proxy 执行期间改变的运行时信息的结构。当增删 DynamicSetting 时，它的规则是：

1. 如果目标 Setting 需要的数据类型（RuntimeDataClass）没变，就保留现有的 RuntimeData。
2. 只有在 RuntimeData 不存在，或者 RuntimeDataClass 发生变化时，它才会去重建。

### 代理是怎么知道配置变了的？

当你添加或移除 DynamicSetting 后，系统会在“不改变执行流”的前提下，通过 `NotifyProxyDynamicSettingChanged` 把这四个信息广播给所有正在干活的 Proxy：

1. `SettingTypeName`
2. `PreviousSetting`
3. `CurrentSetting`
4. `RuntimeData`*

Proxy 收到通知后，可以自行决定是否需要重置或修正某些内部状态。例如内置的 `LookAt` 代理，在收到通知后会重置当前的朝向缓存和插值速度；而 `RotationLag` 会重置旧的旋转缓存。
*(如果你在蓝图里写自定义 Proxy，可以使用插件提供的 `ReceiveDynamicSettingChanged` 事件)*

### 适合什么场景？

只要**执行流本身不变，只是参数需要临时变化**，就该用它。比如：

- 角色进入特定区域，需要临时拉大 FOV 或调整跟随距离。
- 给当前的代理流程叠加一个短时的，仅更改参数的效果。

---

## 三、对比表格

关键问题是**要换“做事的人及顺序”（Proxy），还是仅仅想改“他们读到的参数”（Setting）？**

| 对比项                                    | Change Proxy State (换人) | Add Dynamic Setting (换参数)         |
|:-------------------------------------- |:--------------------------------------------------- |:--------------------------------- |
| **到底改变了什么？**                           | `Proxy` 的执行链                                        | 某类 `Setting` 的有效配置                |
| **会改变执行步骤吗？**                          | **会**                                               | 绝对不会                              |
| **会触发 Proxy 的 Activate/Deactivate 吗？** | **会**                                               | 不会，生命周期维持原状                       |
| **状态变化的感知方式**                          | 通过完整的代理生命周期处理                                       | 通过 `OnDynamicSettingChanged` 通知事件 |
| **典型心智负担**                             | 旧状态是否在 `Deactivate` 里清理干净了？                         | 换配置时，`RuntimeData` 是留用还是重置？       |

---

## 四、仅修改运行时信息（如插值起点/终点）

事实上，`Add Dynamic Setting` 并不是修改运行时参数的唯一方式。
如果你不是想替换掉一整个 `Setting` 资产，而只是想修改某个**已经在运行中的临时状态**（比如每一帧更新锁定目标的具体坐标），最直接的方法是：**直接取 RuntimeData 出来改**

例如一个继承于 OasisCameraComponent 的相机组件想要修改相机锁定点可以新增函数`UMyCameraComponent::SetLookAtPosition` ：
函数内先通过 `GetTopModeSettingRuntimeData(LookAt)` 拿到运行时的 `UOasisCameraSettingRuntimeData_LookAt` 实例，然后直接把新的目标点坐标写进去。

所以，最终的扩展边界划分其实是这样的：

1. **Change Proxy State**：换执行流。
2. **Add Dynamic Setting**：换一份当前生效的静态配置表。
3. **直接改 RuntimeData**：微调当前配置表正在使用的临时活动数据。

---

## 五、插件静态扩展建议

如果准备新增自定义的 Proxy 或 Setting，请考虑如下建议：

1. **写 Proxy 时：**
   复写如下函数操作该 Proxy 关注的运行时信息：`Activate / Deactivate`和 `OnDynamicSettingChanged`。

2. **写 Setting 时：**
   严格遵守“静态配置”和“运行时记忆”的分离原则。
   
   - `Setting` 类里只放那些配置好就不会动的死参数。
   - 插值进度、存值、动态目标等每帧都在变化的运行时状态应该放到 `RuntimeData` 里去。
     
     

## 六、视频示例

[cameraruntimeextensionexample.mp4](https://github.com/voidczx/ImageBed/blob/main/cameraruntimeextensionexample.mp4 "cameraruntimeextensionexample.mp4")



视频中演示了 ProxyState 的切换和 DynamicSetting 的增加，在实际应用中，这两种运行时拓展方式是可以以叠加关系同时存在的。

视频中通过切换 ProxyState 来屏蔽玩家输入对镜头朝向的影响，同时激活看向固定点的执行代理；通过用 DynamicSetting 覆盖原有 Setting 来实现 FOV 和摇臂距离的增大。
