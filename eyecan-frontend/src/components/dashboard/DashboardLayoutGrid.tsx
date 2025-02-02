import { Grid, GridItem } from "@chakra-ui/react";
import KnowledgeBaseWidget from "@/components/dashboard/knowledgebase/KnowledgeBaseWidget.tsx";
import DashboardHeader from "@/components/dashboard/DashboardHeader.tsx";
import SideBar from "@/components/dashboard/sidebar/SideBar.tsx";
import ChartWidget from "@/components/dashboard/chart/ChartWidget.tsx";

function DashboardLayoutGrid() {
    return (
        <Grid maxHeight="100%" w="100%" templateColumns="repeat(36, 1fr)" templateRows="repeat(24, 1fr)" gap={4}>
            <GridItem colSpan={36} rowSpan={2} area="navBar" display="flex"
                      justifyContent="center"
                      maxHeight="100%">
                <DashboardHeader/>
            </GridItem>
            <GridItem area="chartWidget" colSpan={24} rowSpan={12} paddingLeft={14} overflow="hidden" maxHeight="100%">
                <ChartWidget />
            </GridItem>
            <GridItem area="sideWidgets" colSpan={11} rowSpan={24} paddingLeft={8} paddingBottom={10} maxHeight="100%">
                <SideBar/>
            </GridItem>
            <GridItem colSpan={24} rowSpan={11} area="knowledgeBaseWidget"  paddingLeft={14} paddingBottom={10} paddingTop={6} overflow="hidden" maxHeight="100%">
            <KnowledgeBaseWidget/>
            </GridItem>
        </Grid>
    )
}

export default DashboardLayoutGrid;