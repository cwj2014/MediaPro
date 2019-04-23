//
// Created by yu.cai on 2019/4/16.
//
#include "ffmpeg_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif
    //ffmpeg.c中的所修改成普通函数的main函数
    extern int run_ffmepg_cmd(int argc, char **argv);
#ifdef __cplusplus
}
#endif
int executeCmd(std::vector<std::string> vec){
    std::vector<std::string> _params;
    _params.push_back("ffmpeg");
    if(vec.size()>0)
        _params.insert(_params.begin()+1, vec.begin(),vec.end());
    int argc = _params.size();
    char **argv = (char**)malloc(argc*sizeof(char*));
    for(int i=0; i<argc; i++) {
        std::string param = _params[i];
        argv[i] = (char*)malloc(param.length() + 1);
        strcpy(argv[i], param.c_str());
    }
    int ret = run_ffmepg_cmd(argc, argv);
    for(int i=0; i<argc; i++){
        free(argv[i]);
    }
    free(argv);
    return ret;
}

