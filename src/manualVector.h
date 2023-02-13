//
//  manualVector.h
//  Oceanode
//
//  Created by Eduard Frigola Bagué on 05/10/2020.
//

#ifndef manualVector_h
#define manualVector_h

#define IMGUI_DEFINE_MATH_OPERATORS

#include "ofxOceanodeNodeModel.h"
#include "imgui_internal.h"

class manualVector : public ofxOceanodeNodeModel{
public:
    manualVector() : ofxOceanodeNodeModel("Manual Vector"){}
    
    void setup(){
        addParameter(size.set("Size", 10, 2, INT_MAX));
        addParameter(minVal.set("Min", 0, -FLT_MAX, FLT_MAX));
        addParameter(maxVal.set("Max", 1, -FLT_MAX, FLT_MAX));
        
        addCustomRegion(customWidget, [this](){
            auto values_getter = [](void* data, int idx)-> float
            {
                const float v = *(const float*)(const void*)((const unsigned char*)data + (size_t)idx * sizeof(float));
                return v;
            };
            
//            vectorValue[slot] = vectorValueParam;
            
            auto cursorPos = ImGui::GetCursorScreenPos();
            
            ImGui::InvisibleButton("##InvBox", ImVec2(210, ImGui::GetFrameHeight() * 2)); //Used to check later behaviours
            
            auto drawList = ImGui::GetWindowDrawList();
            
            void* data = (void*)vectorValue.data();
            int values_offset = 0;
            float scale_min = minVal;
            float scale_max = maxVal;
            int values_count = vectorValueParam->size();
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
                auto mousePosPrev = mousePos - ImGui::GetIO().MouseDelta;
            
                // modify on mouse down
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0, 0))//if (hovered && inner_bb.Contains(mousePos) && press)
                {
                    const float t0 = ImClamp((mousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
                    const float t1 = ImClamp((mousePosPrev.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
                    float nVal0 = 1 - ImClamp((mousePos.y - inner_bb.Min.y) / (inner_bb.Max.y - inner_bb.Min.y), 0.0f, 1.0f);
                    float nVal1 = 1 - ImClamp((mousePosPrev.y - inner_bb.Min.y) / (inner_bb.Max.y - inner_bb.Min.y), 0.0f, 1.0f);
                    int v_idx0 = (int)(t0 * item_count);
                    int v_idx1 = (int)(t1 * item_count);
                    IM_ASSERT(v_idx0 >= 0 && v_idx0 < values_count);
                    IM_ASSERT(v_idx1 >= 0 && v_idx1 < values_count);

                    if(v_idx1 < v_idx0){
                        std::swap(v_idx0, v_idx1);
                        std::swap(nVal0, nVal1);
                    }
                    
                    for(int v_idx = v_idx0; v_idx <= v_idx1; v_idx++){
                        float pctPos = 0;
                        if(v_idx0 != v_idx1){
                            pctPos = float(v_idx-v_idx0) / float(v_idx1-v_idx0);
                        }
                        vectorValue[v_idx] = ofMap(ofLerp(nVal0, nVal1, pctPos), 0, 1, scale_min, scale_max, true);
                        if(ImGui::GetIO().KeyShift) vectorValue[v_idx] = round(vectorValue[v_idx]);
                    }
                    
                    idx_hovered = v_idx0;
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
                    if(currentToEditValue > 0){
                        ImGui::SameLine();
                        if(ImGui::Button("<<")){
                            currentToEditValue--;
                        }
                    }
                    if(currentToEditValue < size-1){
                        ImGui::SameLine();
                        if(ImGui::Button(">>")){
                            currentToEditValue++;
                        }
                    }
                    ImGui::SliderFloat("##edit", &vectorValue[currentToEditValue], vectorValueParam.getMin()[0], vectorValueParam.getMax()[0], "%.4f");
                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                
                vectorValueParam = vectorValue;
                
            }
            
        });
        
        vectorValue.resize(size, 0);
        
        addParameter(vectorValueParam.set("Out", vectorValue, vector<float>(1, minVal), vector<float>(1, maxVal)), ofxOceanodeParameterFlags_DisableInConnection | ofxOceanodeParameterFlags_DisplayMinimized);
        
        listeners.push(size.newListener([this](int &s){
            vectorValue.resize(size);
            vectorValueParam = vectorValue;
        }));
        
        listeners.push(minVal.newListener([this](float &f){
            for(auto &v : vectorValue) v = ofClamp(v, minVal, maxVal);
            vectorValueParam.setMin(vector<float>(1, f));
            vectorValueParam = vectorValue;
        }));
        
        listeners.push(maxVal.newListener([this](float &f){
            for(auto &v : vectorValue) v = ofClamp(v, minVal, maxVal);
            vectorValueParam.setMax(vector<float>(1, f));
            vectorValueParam = vectorValue;
        }));
    }
    
    void presetSave(ofJson &json){
        json["Value"] = vectorValue;
    };

    void presetRecallAfterSettingParameters(ofJson &json){
        if(json.count("Value") == 1){
            vectorValue = json["Value"].get<vector<float>>();
        }
    };

	void presetHasLoaded() override {
		vectorValueParam = vectorValue;
	};
    
private:
    ofEventListeners listeners;
    
    ofParameter<int> size;
    ofParameter<float> minVal;
    ofParameter<float> maxVal;
    vector<float> vectorValue;
    ofParameter<vector<float>> vectorValueParam;
    customGuiRegion customWidget;
    
    int lastEditValue;
    int currentToEditValue;
};

#endif /* manualVector_h */
