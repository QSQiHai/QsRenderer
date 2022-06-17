# QsRenderer
C++实现小型软件光栅化渲染器。

按照自己的理解复刻了一遍图像渲染管线。



## 使用方法

运行项目，直接在控制台输入要渲染的模型的个数，然后输入模型路径。

在项目目录会生成 output.tga 文件，即为渲染后的结果。

若想支持纹理贴图，则要在 shader 中载入对应的贴图，并且在模型的文件目录下修改对应的后缀名。



## 各个文件介绍

- geometry：几何数学库，实现基本的向量矩阵运算。
- tgaimage：用于读取和写入 .tga 文件，实现纹理贴图的加载和渲染图像的输出。
- model：用于从 .obj 文件中读取顶点数据，包括顶点位置，顶点的法向量，顶点的 uv 纹理坐标。
- camera：用于定义摄像机位置，摄像机朝向，视场大小，横纵比，近平面位置，远平面位置。
- light：目前只支持平行光，用于定义光线方向和颜色。
- buffer：封装二维数组，用于在二维数组中对各种数据进行读取和写入。
- renderer：渲染器主体，实现各种数据的获取以便于 shader 进行着色，控制整个渲染流程。
- shader：通过 renderer 传入各种所需数据，通过 vertex 顶点着色器和 fragment 片元着色器实现各种效果。



## 目前支持的效果



### Blinn-Phong 着色模型

![](https://raw.githubusercontent.com/QSQiHai/QsRenderer/master/images/Blinn-Phong.png)





### 法线贴图



















