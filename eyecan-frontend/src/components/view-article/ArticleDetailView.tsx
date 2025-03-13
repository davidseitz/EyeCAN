import {
    DialogBody,
    DialogCloseTrigger,
    DialogContent,
    DialogFooter,
    DialogHeader,
    DialogRoot,
    DialogTitle,
    DialogTrigger
} from "@/components/ui/dialog.tsx";
import {Button, HStack, Text} from "@chakra-ui/react";
import React from "react";
import {Article} from "@/types/article.ts";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";

interface ArticleDetailViewProps {
    children: React.ReactNode;
    article: Article;
}

function ArticleDetailView({children, article}: ArticleDetailViewProps) {
    return (
        <DialogRoot size="cover" placement="center">
            <DialogTrigger>
                {children}
            </DialogTrigger>
            <DialogContent padding={4} borderRadius={25}>
                <DialogHeader>
                    <HStack justifyContent="space-between" marginRight={6}>
                        <DialogTitle fontSize={35}>{article.title}</DialogTitle>
                        <HStack gap={8} fontStyle="italic" fontWeight="bold" fontSize={12}>
                            <Text>{article.author}</Text>
                            <Text>{article.date}</Text>
                        </HStack>
                    </HStack>
                    <DialogCloseTrigger borderRadius={100}/>
                </DialogHeader>
                <DialogBody maxHeight={"80%"}>
                    <ScrollableContainer height={"100%"} fade={{show: true, size: 75}} >
                        {article.content}
                    </ScrollableContainer>
                </DialogBody>
                <DialogFooter marginTop="auto" marginRight="auto">
                    <Button variant="outline" paddingX={8} size="2xs" borderColor="black" borderRadius={8}>
                        Edit
                    </Button>
                </DialogFooter>
            </DialogContent>
        </DialogRoot>
    )
}

export default ArticleDetailView