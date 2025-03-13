import {StrictMode} from 'react'
import {createRoot} from 'react-dom/client'
import App from './App.tsx'
import './index.css'
import {ColorModeProvider} from "@/components/ui/color-mode.tsx";
import {ChakraProvider, defaultSystem} from "@chakra-ui/react";

createRoot(document.getElementById('root')!).render(
    <StrictMode>
        <ChakraProvider value={defaultSystem}>
            <ColorModeProvider><App/></ColorModeProvider>
        </ChakraProvider>
    </StrictMode>,
)
