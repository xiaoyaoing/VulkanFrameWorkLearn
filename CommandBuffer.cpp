#include "CommandBuffer.h"
#include "Buffer.h"
#include "Image.h"
#include "Descriptor/DescriptorSet.h"

void CommandBuffer::beginRecord(VkCommandBufferUsageFlags usage) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = usage; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional
    if (vkBeginCommandBuffer(_buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command");
    }
}

void CommandBuffer::beginRenderPass(VkRenderPass renderPass, VkFramebuffer buffer,
                                    const std::vector<VkClearValue> &clearValues,
                                    const VkExtent2D &extent2D) {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = buffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent2D;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;


    vkCmdBeginRenderPass(_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::bindVertexBuffer(std::vector<BufferPtr> &buffers, const std::vector<VkDeviceSize> &offsets) {
    auto bufferHandles = getHandles<Buffer, VkBuffer>(buffers);

    vkCmdBindVertexBuffers(_buffer, 0, static_cast<uint32_t>(bufferHandles.size()), bufferHandles.data(),
                           offsets.data());
}

void CommandBuffer::bindIndicesBuffer(const BufferPtr &buffer, VkDeviceSize offset) {
    // auto bufferHandles = getHandles<Buffer,VkBuffer>(buffers);
    vkCmdBindIndexBuffer(_buffer, buffer->getHandle(), offset, VK_INDEX_TYPE_UINT16);
}

void CommandBuffer::bindDescriptorSets(VkPipelineBindPoint bindPoint, VkPipelineLayout layout, uint32_t firstSet,
                                       const std::vector<ptr<DescriptorSet>> &descriptorSets,
                                       const std::vector<uint32_t> &dynamicOffsets) {
    auto vkDescriptorSets = getHandles<DescriptorSet, VkDescriptorSet>(descriptorSets);
    vkCmdBindDescriptorSets(_buffer, bindPoint, layout, firstSet, vkDescriptorSets.size(), vkDescriptorSets.data(),
                            dynamicOffsets.size(), dynamicOffsets.data());
}

void CommandBuffer::copyBufferToImage(ptr<Buffer> src, ptr<Image> dst, const std::vector<VkBufferImageCopy> &copyRegions) {
    vkCmdCopyBufferToImage(_buffer,
                           src->getHandle(), dst->getHandle(),
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           static_cast<uint32_t>(copyRegions.size()), copyRegions.data());
}
