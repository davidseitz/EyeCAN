import KbHeader from "@/components/dashboard/knowledgebase/KbHeader.tsx";
import ArticlePreviewCard from "@/components/dashboard/knowledgebase/article-preview/ArticlePreviewCard.tsx";
import mockData from "@/data/MockData.ts";
import {VStack} from "@chakra-ui/react";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";

function KnowledgeBaseWidget() {
    return (
        <VStack maxHeight="100%" overflowY="hidden">
            <KbHeader/>
            <ScrollableContainer fade={{show: true, size: 100}}>
                {mockData.articles.map((article) =>
                    <ArticlePreviewCard key={article.id} article={article}/>)}
            </ScrollableContainer>
        </VStack>

    )
}


export default KnowledgeBaseWidget;