import KbHeader from "@/components/dashboard/knowledgebase/KbHeader.tsx";
import ArticlePreviewCard from "@/components/dashboard/knowledgebase/article-preview/ArticlePreviewCard.tsx";
import mockData from "@/data/MockData.ts";
import {VStack} from "@chakra-ui/react";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";
import ArticleDetailView from "@/components/view-article/ArticleDetailView.tsx";

function KnowledgeBaseWidget() {
    return (
        <VStack maxHeight="100%" overflowY="hidden">
            <KbHeader/>
            <ScrollableContainer fade={{show: true, size: 100}}>
                {mockData.articles.map((article) =>
                    <ArticleDetailView article={article}>
                        <ArticlePreviewCard key={article.id} article={article}/>
                    </ArticleDetailView>
                )}
            </ScrollableContainer>
        </VStack>

    )
}


export default KnowledgeBaseWidget;