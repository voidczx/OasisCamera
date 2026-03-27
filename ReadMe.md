# 简介

OasisCameraPlugin 是一个蓝图拓展友好的，对于 3D 第三人称镜头情景下可开箱即用的镜头插件。

整体思路是用 CameraMode 来存储相机每次更新的位置、朝向和 FOV 所需的静态配置和执行流，用户仅需拓展静态配置类型和执行流类型，再在各自业务中使用的 CameraMode 进行拼装即可。



# 快速开始

1. 使用 UE 原生的 PlayerCameraManager

2. 在 PlayerController 所 Possess 的 Pawn 蓝图上挂载上位于插件 Content 目录下的 ExampleCameraComponent

3. Play in your Editor
   
   

# 蓝图拓展

## 静态配置

1. 创建继承于 OasisCameraSetting_BlueprintBase 的蓝图 MySettingClass，并为该类静态配置起一个名字填入 Setting Type Name 属性

2. 创建继承于 OasisCameraSettingRuntimeData_BlueprintBase 的蓝图 MyRuntimeDataClass，该结构用于存储用于同一 CameraMode 中所有执行流共享的，处理静态配置时所需的运行时信息。

3. 在第 1 步中创建的 MySettingClass 的 Runtime Data Class 属性中选择第 2 步创建的 MyRuntimeDataClass

4. 在要使用的 CameraMode 里的 Instance Mode Settings 里增加 MySettingClass 类型的配置



## 执行流

1. 创建继承于 OasisCameraProxy_BlueprintBase 的蓝图 MyCameraProxy

2. 找到要使用的 CameraMode 里的 Proxy Classes 属性，在该数据集中的 Class List 里选择合适的执行位置插入新增的执行流 MyCameraProxy














