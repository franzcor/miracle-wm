#ifndef MIRACLE_POLICY_H
#define MIRACLE_POLICY_H

#include "output_content.h"
#include "miracle_config.h"
#include "workspace_manager.h"
#include "ipc.h"
#include "window_manager_tools_tiling_interface.h"
#include "window_metadata.h"

#include <miral/window_manager_tools.h>
#include <miral/window_management_policy.h>
#include <miral/external_client.h>
#include <miral/internal_client.h>
#include <miral/output.h>
#include <miral/minimal_window_manager.h>
#include <memory>
#include <vector>

namespace miral
{
class MirRunner;
}

namespace miracle
{

class Policy : public miral::WindowManagementPolicy
{
public:
    Policy(
        miral::WindowManagerTools const&,
        miral::ExternalClientLauncher const&,
        miral::MirRunner&,
        std::shared_ptr<MiracleConfig> const&);
    ~Policy() override;

    bool handle_keyboard_event(MirKeyboardEvent const* event) override;
    bool handle_pointer_event(MirPointerEvent const* event) override;
    auto place_new_window(
        miral::ApplicationInfo const& app_info,
        miral::WindowSpecification const& requested_specification) -> miral::WindowSpecification override;
    void advise_new_window(miral::WindowInfo const& window_info) override;
    void handle_window_ready(miral::WindowInfo& window_info) override;
    void advise_focus_gained(miral::WindowInfo const& window_info) override;
    void advise_focus_lost(miral::WindowInfo const& window_info) override;
    void advise_delete_window(miral::WindowInfo const& window_info) override;
    void advise_move_to(miral::WindowInfo const& window_info, geom::Point top_left) override;
    void advise_output_create(miral::Output const& output) override;
    void advise_output_update(miral::Output const& updated, miral::Output const& original) override;
    void advise_output_delete(miral::Output const& output) override;
    void handle_modify_window(miral::WindowInfo &window_info, const miral::WindowSpecification &modifications) override;

    void handle_raise_window(miral::WindowInfo &window_info) override;

    auto confirm_placement_on_display(
        const miral::WindowInfo &window_info,
        MirWindowState new_state,
        const mir::geometry::Rectangle &new_placement) -> mir::geometry::Rectangle override;

    bool handle_touch_event(const MirTouchEvent *event) override;

    void handle_request_move(miral::WindowInfo &window_info, const MirInputEvent *input_event) override;

    void handle_request_resize(
        miral::WindowInfo &window_info,
        const MirInputEvent *input_event,
        MirResizeEdge edge) override;

    auto confirm_inherited_move(
        const miral::WindowInfo &window_info,
        mir::geometry::Displacement movement) -> mir::geometry::Rectangle override;

    void advise_application_zone_create(miral::Zone const& application_zone) override;
    void advise_application_zone_update(miral::Zone const& updated, miral::Zone const& original) override;
    void advise_application_zone_delete(miral::Zone const& application_zone) override;

    std::shared_ptr<OutputContent> const& get_active_output() { return active_output; }

private:
    std::shared_ptr<OutputContent> active_output;
    std::vector<std::shared_ptr<OutputContent>> output_list;
    std::weak_ptr<OutputContent> pending_output;
    WindowType pending_type;
    std::vector<Window> orphaned_window_list;
    miral::WindowManagerTools window_manager_tools;
    miral::MinimalWindowManager floating_window_manager;
    miral::ExternalClientLauncher const external_client_launcher;
    miral::MirRunner& runner;
    std::shared_ptr<MiracleConfig> config;
    WorkspaceObserverRegistrar workspace_observer_registrar;
    WorkspaceManager workspace_manager;
    std::shared_ptr<Ipc> ipc;
    WindowManagerToolsTilingInterface node_interface;
};
}

#endif //MIRACLE_POLICY_H
