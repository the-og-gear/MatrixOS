#include "UAD.h"

bool UAD::SetRegister(ActionInfo* actionInfo, uint32_t value)
{
    return registers.insert_or_assign(*actionInfo, value).second;
}

bool UAD::GetRegister(ActionInfo* actionInfo, uint32_t* value)
{   
    std::unordered_map<ActionInfo, uint32_t>::iterator it = registers.find(*actionInfo);

    if(it == registers.end())
    {
        return false;
    }

    *value = it->second;
    return true;
}

bool UAD::ClearRegister(ActionInfo* actionInfo)
{
    return registers.erase(*actionInfo) > 0;
}

  void UAD::SetLayerState(uint8_t layer, LayerInfoType type, bool state)
  {
    if(type == LayerInfoType::ACTIVE)
    {
        if(state)
        {
            layerEnabled |= (1 << layer);
        }
        else
        {
            layerEnabled &= ~(1 << layer);
        }
    }
    else if(type == LayerInfoType::PASSTHROUGH)
    {
        if(state)
        {
            layerPassthough |= (1 << layer);
        }
        else
        {
            layerPassthough &= ~(1 << layer);
        }
    }
  }

bool UAD::GetLayerState(uint8_t layer, LayerInfoType type)
{
    if(type == LayerInfoType::ACTIVE)
    {
        return IsBitSet(layerEnabled, layer);
    }
    else if(type == LayerInfoType::PASSTHROUGH)
    {
        return IsBitSet(layerPassthough, layer);
    }
    return false;
}

int8_t UAD::IndexInBitmap(uint64_t bitmap, uint8_t index)
{
    if(!IsBitSet(bitmap, index))
    {
        return -1;
    }

    // Find nums of bits set before index - TODO: This can probably be opttimized by a lot
    uint8_t count = 0;
    for (uint8_t i = 0; i < index; i++)
    {
        count += IsBitSet(bitmap, i);
    }
    return count + 1;
}