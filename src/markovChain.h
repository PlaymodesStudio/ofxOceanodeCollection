//
//  markovChain.h
//  Oceanode
//
//  Created by Eduard Frigola Bagué on 04/11/2020.
//

#ifndef markovChain_h
#define markovChain_h

#include "ofxOceanodeNodeModel.h"
#include "imgui_internal.h"
#include <random>

class markovChain : public ofxOceanodeNodeModel{
public:
    markovChain() : ofxOceanodeNodeModel("Markov Chain"){}
    
    void setup(){
        addParameter(phasorIn.set("Phase", 0, 0, 1));
        addParameter(size.set("Size", 2, 2, 100));
        addParameter(state.set("State", 0, 0, size-1));
        //addParameter(minVal.set("Min", 0, -1000, 1000));
        //addParameter(maxVal.set("Max", 1, -1000, 1000));
        
        addCustomRegion(customWidget, [this](){
            auto values_getter = [](void* data, int idx)-> float
            {
                const float v = *(const float*)(const void*)((const unsigned char*)data + (size_t)idx * sizeof(float));
                return v;
            };
            
//            vectorValue[state] = vectorValueParam;
            
            auto cursorPos = ImGui::GetCursorScreenPos();
            
            ImGui::InvisibleButton("##InvBox", ImVec2(210, ImGui::GetFrameHeight() * 2)); //Used to check later behaviours
            
            auto drawList = ImGui::GetWindowDrawList();
            
            void* data = (void*)vectorValue[state].data();
            int values_offset = 0;
            float scale_min = 0.0f;
            float scale_max = 1.0f;
            int values_count = vectorValue[state].size();
            ImVec2 frame_size = ImVec2(210, ImGui::GetFrameHeight() * 2);
            
            
            const ImGuiStyle& style = ImGui::GetStyle();
            const ImRect frame_bb(cursorPos, cursorPos + frame_size);
            const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
            
            ImGui::RenderFrame(inner_bb.Min, inner_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

            bool hovered = ImGui::IsItemHovered();
            bool press = ImGui::IsMouseDown(0);
            
            int idx_hovered = -1;
            if (values_count >= 1)
            {
                int res_w = ImMin((int)frame_size.x, values_count);
                int item_count = values_count;

                auto mousePos = ImGui::GetIO().MousePos;
                // modify on mouse down
                if (ImGui::IsItemActive() && ImGui::IsMouseDown(0))//if (hovered && inner_bb.Contains(mousePos) && press)
                {
                    const float t = ImClamp((mousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
                    const float nVal = 1 - ImClamp((mousePos.y - inner_bb.Min.y) / (inner_bb.Max.y - inner_bb.Min.y), 0.0f, 1.0f);
                    const int v_idx = (int)(t * item_count);
                    IM_ASSERT(v_idx >= 0 && v_idx < values_count);

                    vectorValue[state][v_idx] = ofMap(nVal, 0, 1, scale_min, scale_max, true);
                    if(ImGui::GetIO().KeyShift) vectorValue[state][v_idx] = (int)vectorValue[state][v_idx];
                    
                    idx_hovered = v_idx;
                }
                
                if(ImGui::IsItemClicked(1) || (ImGui::IsPopupOpen("Value Popup") && ImGui::IsMouseClicked(1))){
                    ImGui::OpenPopup("Value Popup");
                    const float t = ImClamp((mousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
                    const int v_idx = (int)(t * item_count);
                    IM_ASSERT(v_idx >= 0 && v_idx < values_count);
                    currentToEditValue = v_idx;
                }

                const float t_step = 1.0f / (float)res_w;
                const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

                float v0 = values_getter(data, (0 + values_offset) % values_count);
                float t0 = 0.0f;
                ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
                float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

                const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram);
                const ImU32 col_hovered = ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered);

                for (int n = 0; n < res_w; n++)
                {
                    const float t1 = t0 + t_step;
                    const int v1_idx = (int)(t0 * item_count + 0.5f);
                    IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
                    const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
                    const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

                    // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
                    ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
                    ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, ImVec2(tp1.x, histogram_zero_line_t));
                    
//                    else if (plot_type == ImGuiPlotType_Histogram)
                    {
                        if (pos1.x >= pos0.x + 2.0f)
                            pos1.x -= 1.0f;
                        drawList->AddRectFilled(pos0, pos1, idx_hovered == v1_idx ? col_hovered : col_base);
                    }

                    t0 = t1;
                    tp0 = tp1;
                }
                
                
                
                
//                 if(idx_hovered != -1) lastEditValue = idx_hovered;
//                 int drag = 0;
//                bool resetValue = false;

//                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)){
//                    resetValue = true;
//                    vectorValue[idx_hovered] = 0.5;
//                    vectorValueParam = vectorValue;
//                }

                if (ImGui::BeginPopupContextItem("Value Popup"))
                {
                    ImGui::Text("%s", ("Edit item " + ofToString(currentToEditValue)).c_str());
                    ImGui::SliderFloat("##edit", &vectorValue[state][currentToEditValue], 0.0f, 1.0f, "%.4f");
                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                
                //vectorValueParam = vectorValue[state];
                
            }
            
        });
        
        vectorValue.resize(size, vector<float>(size, 0));
        //vectorValue[state].resize(size, 0);
        
        addParameter(result.set("Result", 0, 0, size-1), ofxOceanodeParameterFlags_DisableInConnection);
        
        listeners.push(size.newListener([this](int &s){
            for(auto &v : vectorValue) v.resize(size);
            vectorValue.resize(size, vector<float>(size, 0));
            state.setMax(size-1);
            result.setMax(size-1);
        }));
    
        
        listeners.push(state.newListener([this](int &stateInx){
            //size = vectorValue[stateInx].size();
        }));
        
        listeners.push(phasorIn.newListener([this](float &f){
            if(f < oldPhasor){ //Phasor cycle
                std::discrete_distribution<int> dist(vectorValue[state].begin(), vectorValue[state].end());
                result = dist(mt);
            }
            oldPhasor = f;
        }));
        
        oldPhasor = -1;
        
        std::random_device rd;
        mt.seed(rd());
    }
    
    void presetSave(ofJson &json){
        for(int i = 0; i < size; i++){
            json["Values"][ofToString(i)] = vectorValue[i];
        }
    };

    void presetRecallAfterSettingParameters(ofJson &json){
        if(json.count("Values") == 1){
            for(int i = 0; i < size; i++){
                vectorValue[i] = json["Values"][ofToString(i)].get<vector<float>>();
            }
        }
    };
    
private:
    ofEventListeners listeners;
    
    ofParameter<float> phasorIn;
    ofParameter<int> state;
    ofParameter<int> size;
    //ofParameter<float> minVal;
    //ofParameter<float> maxVal;
    vector<vector<float>> vectorValue;
    ofParameter<int> result;
    customGuiRegion customWidget;
    
    int lastEditValue;
    int currentToEditValue;
    
    float oldPhasor;
    
    //std::discrete_distribution<int> disdist(customDiscreteDistribution.begin(), customDiscreteDistribution.end());
    int seed;
    std::mt19937 mt;
};

#endif /* markovChain_h */
