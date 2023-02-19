#ifndef PTO_RENDERER_VIEW_HARDWAREPATHTRACING_HPP
#define PTO_RENDERER_VIEW_HARDWAREPATHTRACING_HPP

#include <vzt/Utils/Compiler.hpp>
#include <vzt/Vulkan/AccelerationStructure.hpp>
#include <vzt/Vulkan/Buffer.hpp>
#include <vzt/Vulkan/Command.hpp>
#include <vzt/Vulkan/Pipeline/RaytracingPipeline.hpp>

#include "pto/Renderer/Geometry.hpp"
#include "pto/Renderer/Sky.hpp"
#include "pto/System/System.hpp"

namespace pto
{

    class HardwarePathTracingView
    {
      public:
        struct Properties
        {
            vzt::Mat4 view;
            vzt::Mat4 projection;
            uint32_t  sampleId;
        };

        HardwarePathTracingView(vzt::View<vzt::Device> device, uint32_t imageNb, vzt::Extent2D extent, System& system,
                                vzt::View<GeometryHandler> handler, Sky sky);
        ~HardwarePathTracingView() = default;

        void resize(vzt::Extent2D extent);
        void update();

        void record(uint32_t imageId, vzt::CommandBuffer& commands, const vzt::View<vzt::DeviceImage> outputImage,
                    const Properties& properties);

      private:
        vzt::View<vzt::Device> m_device;
        uint32_t               m_imageNb;

        vzt::ShaderGroup        m_shaderGroup;
        vzt::Compiler           m_compiler{};
        vzt::DescriptorLayout   m_layout;
        vzt::RaytracingPipeline m_pipeline;

        std::vector<vzt::DeviceImage> m_accumulationImages;
        std::vector<vzt::ImageView>   m_accumulationImageView;
        std::vector<vzt::DeviceImage> m_renderImages;
        std::vector<vzt::ImageView>   m_renderImageView;

        vzt::DescriptorPool m_descriptorPool;
        std::size_t         m_uboAlignment;
        vzt::Buffer         m_ubo;
        vzt::Buffer         m_objectDescriptionBuffer;

        uint32_t    m_handleSizeAligned;
        uint32_t    m_handleSize;
        vzt::Buffer m_raygenShaderBindingTable;
        vzt::Buffer m_missShaderBindingTable;
        vzt::Buffer m_hitShaderBindingTable;

        vzt::Extent2D              m_extent;
        System*                    m_system;
        vzt::View<GeometryHandler> m_handler;
        Sky                        m_sky;
    };
} // namespace pto

#endif // PTO_RENDERER_VIEW_HARDWAREPATHTRACING_HPP
