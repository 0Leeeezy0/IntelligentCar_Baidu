该文件夹内的`include`文件夹下的文件复制到`/usr/local/include/onnx`下

该文件夹内的`lib`文件夹下的文件复制到`/usr/local/lib/onnx`下

必须在linux系统下操作，此路径皆为linux根文件系统路径

可用如下命令进行操作
```
sudo mkdir /usr/local/include/onnx
sudo mkdir /usr/local/lib/onnx
sudo cp -r ./include /usr/local/include/onnx
sudo cp -r ./include /usr/local/lib/onnx

```

该文件夹下的库皆为x86架构，不支持ARM架构，如使用ARM架构平台请自行移植

