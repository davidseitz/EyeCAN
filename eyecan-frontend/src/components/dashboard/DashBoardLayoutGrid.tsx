import { Grid, GridItem} from "@chakra-ui/react";

function DashBoardLayoutGrid() {
    return (
        <Grid h="100%" w="100%" templateColumns="repeat(36, 1fr)" templateRows="repeat(2, 1fr)" gap={4}>
            <GridItem bg="green" colSpan={24} rowSpan={1}>
            </GridItem>
            <GridItem bg="green" colSpan={12} rowSpan={2}>
            </GridItem>
            <GridItem bg="green" colSpan={24} rowSpan={1}>
            </GridItem>
        </Grid>
    )
}

export default DashBoardLayoutGrid;