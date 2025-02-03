import FilterCollection from "@/components/dashboard/sidebar/filter-collection/FilterCollection.tsx";
import ActionButtons from "@/components/dashboard/sidebar/action-buttons/ActionButtons.tsx";
import DataCollection from "@/components/dashboard/sidebar/data-collection/DataCollection.tsx";

function SideBar() {
    return (
        <>
            <FilterCollection/>
            <DataCollection/>
            <ActionButtons/>
        </>
    )
}

export default SideBar;