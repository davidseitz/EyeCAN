import { Grid, GridItem} from "@chakra-ui/react";
import KnowledgeBaseWidget from "@/components/dashboard/knowledgebase/KnowledgeBaseWidget.tsx";

function DashBoardLayoutGrid() {
    return (
        <Grid h="100%" w="100%" templateColumns="repeat(36, 1fr)" templateRows="repeat(2, 1fr)" gap={4}>
            <GridItem area="chartWidget" colSpan={24} rowSpan={1}>
            </GridItem>
            <GridItem area="sideWidgets" colSpan={12} rowSpan={2}>
            </GridItem>
            <GridItem paddingLeft={14} area="knowledgeBaseWidget" colSpan={24} rowSpan={1}>
                <KnowledgeBaseWidget/>
            </GridItem>
        </Grid>
    )
}

export default DashBoardLayoutGrid;