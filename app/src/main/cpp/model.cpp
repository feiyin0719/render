#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"
#include <android/log.h>

#define TAG "HELLO"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)
Model::Model(const std::string data)
        : verts_(), uv_(), norms_(), facet_vrt_(), facet_tex_(), facet_nrm_(), diffusemap_(),
          normalmap_(), specularmap_() {
    std::stringstream in(data);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {

        std::getline(in, line);
        LOGI("read:%s",line.c_str());
        if(line.empty())continue;
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        }
//        else if (!line.compare(0, 3, "vn ")) {
//            iss >> trash >> trash;
//            vec3f n;
//            for (int i = 0; i < 3; i++) iss >> n[i];
//            norms_.push_back(n.normalize());
//        }
//        else if (!line.compare(0, 3, "vt ")) {
//            iss >> trash >> trash;
//            vec2f uv;
//            for (int i = 0; i < 2; i++) iss >> uv[i];
//            uv_.push_back(uv);
//        }
        else if (!line.compare(0, 2, "f ")) {
            int f, t, n;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                facet_vrt_.push_back(--f);
                facet_tex_.push_back(--t);
                facet_nrm_.push_back(--n);
                cnt++;
            }
            if (3 != cnt) {
//                std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;

                return;
            }
        }
    }

//    std::cerr << "# v# " << nverts() << " f# "  << nfaces() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
//    load_texture(filename, "_diffuse.tga",    diffusemap_);
//    load_texture(filename, "_nm_tangent.tga", normalmap_);
//    load_texture(filename, "_spec.tga",       specularmap_);
}

int Model::nverts() const {
    return verts_.size();
}

int Model::nfaces() const {
    return facet_vrt_.size() / 3;
}



vec3f Model::vert(const int i) const {
    return verts_[i];
}

vec3f Model::vert(const int iface, const int nthvert) const {
    return verts_[facet_vrt_[iface * 3 + nthvert]];
}

void Model::load_texture(std::string filename, const std::string suffix, TGAImage &img) {
    size_t dot = filename.find_last_of(".");
    if (dot == std::string::npos) return;
    std::string texfile = filename.substr(0, dot) + suffix;
    std::cerr << "texture file " << texfile << " loading "
              << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
    img.flip_vertically();
}

TGAColor Model::diffuse(const vec2f &uvf) const {
    return diffusemap_.get(uvf[0] * diffusemap_.get_width(), uvf[1] * diffusemap_.get_height());
}

vec3f Model::normal(const vec2f &uvf) const {
    TGAColor c = normalmap_.get(uvf[0] * normalmap_.get_width(), uvf[1] * normalmap_.get_height());
    vec3f res;
    for (int i = 0; i < 3; i++)
        res[2 - i] = c[i] / 255. * 2 - 1;
    return res;
}

double Model::specular(const vec2f &uvf) const {
    return specularmap_.get(uvf[0] * specularmap_.get_width(),
                            uvf[1] * specularmap_.get_height())[0];
}

vec2f Model::uv(const int iface, const int nthvert) const {
    return uv_[facet_tex_[iface * 3 + nthvert]];
}

vec3f Model::normal(const int iface, const int nthvert) const {
    return norms_[facet_nrm_[iface * 3 + nthvert]];
}

