#include "DescriptorSet.h"
#include "DescriptorLayout.h"
#include "DescriptorPool.h"
#include <Buffer.h>
#include <Device.h>

DescriptorSet::DescriptorSet(const ptr<Device>& device, const ptr<DescriptorPool>& descriptorPool,
                             const ptr<DescriptorLayout>& descriptorSetLayout, const uint32_t descSetCount) : _device(device) {
     _device = device;
//    _descriptorPool = descriptorPool;
//    _descriptorSetLayout = descriptorSetLayout;

    const VkDescriptorSetLayout setLayout = descriptorSetLayout->getHandle();
    VkDescriptorSetAllocateInfo descSetAllocInfo{};
    descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descSetAllocInfo.pNext = nullptr;
    descSetAllocInfo.descriptorPool = descriptorPool->getHandle();
    descSetAllocInfo.descriptorSetCount = descSetCount;
    descSetAllocInfo.pSetLayouts = &setLayout;

    ASSERTEQ(vkAllocateDescriptorSets(device->getHandle(), &descSetAllocInfo, &_descriptorSet),VK_SUCCESS);

}

void DescriptorSet::updateBuffer(const std::vector<ptr<Buffer>> &buffers, const uint32_t dstBinding,
                                 const uint32_t descCount, VkDescriptorType descType) {
    std::vector<VkDescriptorBufferInfo> bufferInfos;
    bufferInfos.reserve(buffers.size());
    for (size_t i = 0; i < buffers.size(); ++i)
    {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = buffers[i]->getHandle();
        bufferInfo.offset = 0;
        bufferInfo.range = buffers[i]->getSize();
        bufferInfos.emplace_back(std::move(bufferInfo));
    }

    VkWriteDescriptorSet writeSet = {};
    writeSet.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.pNext				= nullptr;
    writeSet.dstBinding			= dstBinding;
    writeSet.dstSet				= _descriptorSet;
    writeSet.descriptorCount	= descCount;
    writeSet.descriptorType		= descType;
    writeSet.pBufferInfo		= bufferInfos.data();

    vkUpdateDescriptorSets(_device->getHandle(), 1, &writeSet, 0, nullptr);
}
