trusted-ml-hpmpc/
├── Dockerfile                     # Docker 镜像构建文件（用于3方节点）
├── README.md                      # 项目概述 + 执行方式
├── docs/                          # 文档部分（最终提交材料）
│   ├── architecture.md            # 架构设计与系统流程图
│   ├── threat_model.md            # 威胁模型与安全分析
│   ├── assumptions.md             # 信任边界与对手假设
│   └── output_proof.md            # 输出验证结构（含 hash/签名）
├── nn/
│   ├── Pygeon/                    # PyTorch模型 → 二进制导出（提供的工具）
│   │   ├── models/
│   │   ├── data/
│   │   ├── download_pretrained.py
│   │   └── main.py
│   └── PIGEON/                    # C++ 推理引擎（已集成）
├── programs/
│   └── functions/
│       ├── NN.hpp                 # 编译模型推理逻辑（如 function 74）
│       └── custom_501.hpp         # 你们自定义推理功能（function 501）
├── scripts/
│   ├── run.sh                     # 用于多方执行协议（含参数）
│   ├── setup_env.sh               # 可选：快速设置 MODEL_FILE 等环境变量
│   └── measure_connection.sh      # 网络带宽模拟脚本
├── measurements/                  # 用于性能/准确率评估
│   ├── configs/
│   │   └── cifar_benchmark.conf   # 运行自定义 batch、protocol、GPU等配置
│   └── logs/
│       └── output.csv             # 输出评估数据
├── MP-SPDZ/                       # 兼容组件（如果用 MP-SPDZ 编译 .mpc）
│   ├── Bytecodes/
│   └── Schedules/
├── model/                         # 原始 PyTorch 模型结构（辅助训练）
│   └── model.py                   # 可选，仅训练阶段使用
├── .gitignore
└── Makefile                       # 编译功能入口（支持 FUNCTION_IDENTIFIER）

✅ 开发者角色划分建议
模块	任务负责人建议	技术栈
模型训练 + 导出	PyTorch 工程师	Python, Torch, Pygeon
安全协议集成	SMPC 工程师	HPMPC C++, Makefile
执行环境	DevOps	Docker, network, run.sh
文档与提交	文档负责人	Markdown, Draw.io, LaTeX
演示与答辩	项目经理	PPT, live demo, pipeline review

✅ 你们最终交付成果包括：
✅ 可运行的 3 方私有推理系统（含 make+run.sh 成功）

✅ 推理模型精度报告 + 通信耗时 + 加密域执行截图

✅ 安全结构图 + threat_model.md + 签名输出

✅ 文档汇总（作为评委打分核心参考）