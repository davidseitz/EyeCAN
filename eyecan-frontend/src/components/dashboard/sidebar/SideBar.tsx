import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";

function SideBar() {
    return (
        <>
            <DropShadowContainer height={"55%"}/>
            <DropShadowContainer height={"30%"} margins={{marginY: 8}}/>
            <DropShadowContainer height={"15%"} />
        </>
    )
}

export default SideBar;