import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import FilterCollection from "@/components/dashboard/sidebar/filter-collection/FilterCollection.tsx";

function SideBar() {
    return (
        <>
            <FilterCollection/>
            <DropShadowContainer height={"30%"} margins={{marginY: 8}}/>
            <DropShadowContainer height={"15%"} />
        </>
    )
}

export default SideBar;