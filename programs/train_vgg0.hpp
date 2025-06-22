#pragma once
#include "NN.hpp" // 复用已有的VGG结构和数据加载
#include "nn/PIGEON/headers/optimizers.h"
#include "nn/PIGEON/headers/loss_layer.h"
#define FUNCTION train_vgg
#define RESULTTYPE DATATYPE
using namespace simple_nn;
using namespace std;

template <typename Share>
void train_vgg(DATATYPE* res)
{
    using LFLOATTYPE = float;
    using S = XOR_Share<DATATYPE, Share>;
    using A = Additive_Share<DATATYPE, Share>;
    using Bitset = sbitset_t<BITLENGTH, S>;
    using sint = sint_t<A>;
    using modeltype = A;

    // === 配置模型和数据 ===
    int n_train = NUM_INPUTS * BASE_DIV, ch = 3, h = 32, w = 32, num_classes = 10;
    auto model = VGG<modeltype>(num_classes);

    Config cfg;
    cfg.mode = "train";
    cfg.save_dir = "nn/Pygeon/models";
    cfg.data_dir = "nn/Pygeon/data/datasets";
    cfg.pretrained = "dummy";
    cfg.image_file = "CIFAR-10_custom_test_images.bin";
    cfg.label_file = "CIFAR-10_custom_test_labels.bin";
    cfg.batch = NUM_INPUTS * (BASE_DIV);

    // === 加载数据 ===
#if DATAOWNER == -1
    print_online("No Dataowner specified. Loading dummy data...");
    auto train_Y = read_dummy_labels(n_train);
    auto train_X = read_dummy_images(n_train, ch, h, w);
#else
#if PSELF == DATAOWNER
    print_online("Reading dataset from file...");
    string path = cfg.data_dir + "/" + cfg.image_file;
    auto train_X = read_custom_images(path, n_train, ch, h, w);
    print_online("Dataset imported.");
#else
    auto train_X = read_dummy_images(n_train, ch, h, w);
#endif
    string lpath = cfg.data_dir + "/" + cfg.label_file;
    auto train_Y = read_custom_labels(lpath, n_train);
#endif

    MatX<modeltype> train_XX = train_X.unaryExpr([](LFLOATTYPE val) {
        modeltype tmp;
        tmp.template prepare_receive_and_replicate<DATAOWNER>(
            FloatFixedConverter<FLOATTYPE, INT_TYPE, UINT_TYPE, FRACTIONAL>::float_to_ufixed(val));
        return tmp;
    });
    modeltype::communicate();
    for (int i = 0; i < train_XX.size(); i++)
    {
        train_XX(i).template complete_receive_from<DATAOWNER>();
    }
    print_online("Received Secret Share of Dataset");

    DataLoader<modeltype> train_loader;
    train_loader.load(train_XX, train_Y, cfg.batch, ch, h, w, cfg.shuffle_test);

    // === 模型参数秘密共享 ===
    print_online("Compiling model...");
    model.compile({cfg.batch / (BASE_DIV), ch, h, w}, new SGD(0.01, 0.0), new CrossEntropyLoss<modeltype>());

    print_online("Loading model Parameters...");
#if PSELF == MODELOWNER
    print_online("Loading model parameters from file...");
#endif
#if MODELOWNER != -1
    model.template load<MODELOWNER>(cfg.save_dir, cfg.pretrained);
#else
    print_online("No Modelowner specified. Loading dummy parameters...");
#endif
    print_online("Received Secret Share of Model Parameters.");

    // === 训练 ===
    int epochs = 1; // 可根据需要调整
    DataLoader<modeltype> valid_loader; // 占位，暂不做验证
    model.fit(train_loader, epochs, valid_loader);
    print_online("训练完成！");

    // === 安全保存权重 ===
    std::string save_dir = "C:/InDeutschland/202506Hackathon/hpmpc-cinephoto/nn/Pygeon/models/pretrained";
    std::string fname = "vgg_trained_share_P" + std::to_string(PSELF);
    model.save(save_dir, fname);
    print_online("模型权重份额已安全保存到: " + save_dir + "/" + fname);
} 