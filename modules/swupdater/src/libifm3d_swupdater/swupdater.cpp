/*
 * Copyright 2019 ifm electronic, gmbh
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ifm3d/swupdater/swupdater.h>
#include <vector>
#include <ifm3d/camera/camera.h>
#include <ifm3d/camera/err.h>
#include <swupdater_impl.hpp>
#include <swupdater_v2_impl.hpp>

const std::uint16_t ifm3d::SWUPDATER_RECOVERY_PORT = 8080;

namespace ifm3d
{
  auto make_swu_implementor =
    [](ifm3d::CameraBase::Ptr cam,
       const ifm3d::SWUpdater::FlashStatusCb& cb,
       const std::uint16_t swupdate_recovery_port) -> ifm3d::SWUpdater::Impl* {
    switch (cam->SwUpdateVersion())
      {
      case ifm3d::Camera::swu_version::SWU_V1:
        return new ifm3d::SWUpdater::Impl(
          cam,
          cb,
          std::to_string(swupdate_recovery_port));
      case ifm3d::Camera::swu_version::SWU_V2:
        return new ifm3d::ImplV2(cam,
                                 cb,
                                 std::to_string(swupdate_recovery_port));
        break;
      default:
        throw std::runtime_error("swupdate not supported");
        break;
      }
  };
}

ifm3d::SWUpdater::SWUpdater(ifm3d::CameraBase::Ptr cam,
                            const ifm3d::SWUpdater::FlashStatusCb& cb,
                            const std::uint16_t swupdate_recovery_port)
  : pImpl(ifm3d::make_swu_implementor(cam, cb, swupdate_recovery_port))
{}

ifm3d::SWUpdater::~SWUpdater() = default;

void
ifm3d::SWUpdater::RebootToRecovery()
{
  this->pImpl->RebootToRecovery();
}

bool
ifm3d::SWUpdater::WaitForRecovery(long timeout_millis)
{
  return this->pImpl->WaitForRecovery(timeout_millis);
}

void
ifm3d::SWUpdater::RebootToProductive()
{
  this->pImpl->RebootToProductive();
}

bool
ifm3d::SWUpdater::WaitForProductive(long timeout_millis)
{
  return this->pImpl->WaitForProductive(timeout_millis);
}

bool
ifm3d::SWUpdater::FlashFirmware(const std::vector<std::uint8_t>& bytes,
                                long timeout_millis)
{
  return this->pImpl->FlashFirmware(bytes, timeout_millis);
}
