# MSBuild使用

## 命令行生成解决方案

使用`msbuild.exe`命令生成解决方案

```
msbuild tw2_client.sln /p:Configuration="Client_Hybrid" /p:Platform="x64"
```

## 生成指定项目

但是有时候又不想整个解决方案中的项目都生成，只想生成某个指定的项目。可以通过`-target`来指定需要生成的项目，通过`;`间隔不同的项目。

```
msbuild SlnFolders.sln -target:NotInSlnfolder:Rebuild;NewFolder\InSolutionFolder:Clean
```

但是实践中，使用下面命令

```
msbuild tw2_client.sln -target:dummy-test:Build /p:Configuration="Client_Hybrid" /p:Platform="x64"
```

一直提示下面的内容

```
项目“E:\engine_mf\vs2017\apps\tw2_client.sln”在节点 1 上(dummy-test:Build 个目标)。
ValidateSolutionConfiguration:
  正在生成解决方案配置“Client_Hybrid|x64”。
E:\engine_mf\vs2017\apps\tw2_client.sln.metaproj : error MSB4057: 该项目中不存在目标“dummy-test:Build”。 [E:\engine_mf\vs2017\app
s\tw2_client.sln]
已完成生成项目“E:\engine_mf\vs2017\apps\tw2_client.sln”(dummy-test:Build 个目标)的操作 - 失败。
```

查了好久的文档，发现可以通过设置环境变量，来调试查找问题。

```
set MSBUILDEMITSOLUTION=1
```

设置`MSBUILDEMITSOLUTION`环境变量后，执行上面`msbuild`生成命令时，会同时生成对应的`metaproj`文件，`msbuild`最终是依据`metaproj`文档里的内容来执行生成步骤的。

通过查看`tw2_client.sln.metaproj`，发现原来`dummy-test.vcxproj`项目对应的`target`是`plugins\dummy-test`。因为在解决方案中，`dummy-test`项目位于文件夹(filter)`plugins`下...

```xml
<Target Name="plugins\dummy-test" Outputs="@(plugins_dummy-testBuildOutput)">
```

所以上面生成`dummy-test`的命令应该修改成下面形式

```
msbuild tw2_client.sln -target:plugins\dummy-test /p:Configuration="Client_Hybrid" /p:Platform="x64"
```

## 其他

**多核生成加速**

```
  /maxcpucount[:n]   指定用于生成的最大
                    并发进程数。如果未使用开关，则使用的默认值
                    为 1。如果使用开关时不带值，
                    MSBuild 将最多使用计算机上的
                    处理器数。(缩写: /m[:n])
```



## 参考

1. [官方指导文档](https://docs.microsoft.com/en-us/visualstudio/msbuild/walkthrough-using-msbuild?view=vs-2022)
2. [specify-project-file-of-a-solution-using-msbuild](./attachments/specify project file of a solution using msbuild _ Newbedev.pdf)
3. [msbuild troubleshooting](https://docs.microsoft.com/en-us/visualstudio/msbuild/how-to-build-specific-targets-in-solutions-by-using-msbuild-exe?view=vs-2022#troubleshooting)

