import {Grid, GridItem} from "@chakra-ui/react";
import KnowledgeBaseWidget from "@/components/dashboard/knowledgebase/KnowledgeBaseWidget.tsx";
import DashboardHeader from "@/components/dashboard/DashboardHeader.tsx";
import SideBar from "@/components/dashboard/sidebar/SideBar.tsx";
import ChartWidget from "@/components/dashboard/chart/ChartWidget.tsx";

function DashboardLayoutGrid() {
    return (
        <Grid maxHeight="100%" w="100%" maxWidth="1800px" templateColumns="repeat(36, 1fr)" templateRows="repeat(24, 1fr)" gapY={4} gapX={{base: 0, lg: 8}}>
            <GridItem colSpan={36} rowSpan={1} area="navBar" display="flex"
                      justifyContent="center"
                      maxHeight="100%">
                <DashboardHeader/>
            </GridItem>
            <GridItem area="chartWidget" colSpan={{ base: 36, lg: 24 }} rowSpan={12} paddingLeft={14} paddingRight={{base: 14, lg: 0}} overflow="hidden" maxHeight="100%">
                <ChartWidget/>
            </GridItem>
            <GridItem area="sideWidgets" display={{base: "none", lg: "block"}}
                      colSpan={11} rowSpan={24} paddingLeft={8} paddingRight={4} paddingBottom={10} maxHeight="100%">
                <SideBar/>
            </GridItem>
            <GridItem colSpan={{ base: 36, lg: 24 }} rowSpan={11} area="knowledgeBaseWidget" paddingLeft={14} paddingRight={{base: 14, lg: 0}} paddingBottom={10}
                      paddingTop={6} overflow="hidden" maxHeight="100%">
                <KnowledgeBaseWidget/>
            </GridItem>
        </Grid>
    )
}

export default DashboardLayoutGrid;