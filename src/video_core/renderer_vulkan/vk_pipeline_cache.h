// Copyright 2019 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include <boost/functional/hash.hpp>

#include "common/common_types.h"
#include "video_core/engines/maxwell_3d.h"
#include "video_core/renderer_vulkan/declarations.h"
#include "video_core/renderer_vulkan/fixed_pipeline_state.h"
#include "video_core/renderer_vulkan/vk_shader_decompiler.h"
#include "video_core/shader/shader_ir.h"

namespace Vulkan {

class VKDevice;

using Maxwell = Tegra::Engines::Maxwell3D::Regs;

struct GraphicsPipelineCacheKey {
    FixedPipelineState fixed_state;
    std::array<GPUVAddr, Maxwell::MaxShaderProgram> shaders;
    RenderPassParams renderpass_params;

    std::size_t Hash() const noexcept {
        std::size_t hash = fixed_state.Hash();
        for (const auto& shader : shaders) {
            boost::hash_combine(hash, shader);
        }
        boost::hash_combine(hash, renderpass_params.Hash());
        return hash;
    }

    bool operator==(const GraphicsPipelineCacheKey& rhs) const noexcept {
        return std::tie(fixed_state, shaders, renderpass_params) ==
               std::tie(rhs.fixed_state, rhs.shaders, rhs.renderpass_params);
    }
};

struct ComputePipelineCacheKey {
    GPUVAddr shader{};
    u32 shared_memory_size{};
    std::array<u32, 3> workgroup_size{};

    std::size_t Hash() const noexcept {
        return static_cast<std::size_t>(shader) ^
               ((static_cast<std::size_t>(shared_memory_size) >> 7) << 40) ^
               static_cast<std::size_t>(workgroup_size[0]) ^
               (static_cast<std::size_t>(workgroup_size[1]) << 16) ^
               (static_cast<std::size_t>(workgroup_size[2]) << 24);
    }

    bool operator==(const ComputePipelineCacheKey& rhs) const noexcept {
        return std::tie(shader, shared_memory_size, workgroup_size) ==
               std::tie(rhs.shader, rhs.shared_memory_size, rhs.workgroup_size);
    }
};

} // namespace Vulkan

namespace std {

template <>
struct hash<Vulkan::GraphicsPipelineCacheKey> {
    std::size_t operator()(const Vulkan::GraphicsPipelineCacheKey& k) const noexcept {
        return k.Hash();
    }
};

template <>
struct hash<Vulkan::ComputePipelineCacheKey> {
    std::size_t operator()(const Vulkan::ComputePipelineCacheKey& k) const noexcept {
        return k.Hash();
    }
};

} // namespace std

namespace Vulkan {

class VKDevice;

void FillDescriptorUpdateTemplateEntries(
    const VKDevice& device, const ShaderEntries& entries, u32& binding, u32& offset,
    std::vector<vk::DescriptorUpdateTemplateEntry>& template_entries);

} // namespace Vulkan
