#ifndef SG_LABEL
#define SG_LABEL

#include "LinkManager.h"
#include "Node.h"
#include "SnakeGraph.h"
#include <string>

namespace SnakeGraph {

/*
 * Notes on the intended usages of Label:
 * - Labels are supposed to be fast access data that stays in memory for sorting between nodes
 * - Labels are not meant to be directly created or manipulated by users, this datatype should stay within the SnakeGraph library
 * - Labels can be compared to eachother, follows normal C++ conventions for comparrisons
 *
 * Label Types:
 * 0: No data
 * 1: String data
 * 2: 32bit Int
 * 3: Double
 *
 * Label Encoding Standard:
 * <label_type>:<label_title>:<value>
 */

    class Label {
        private:
            std::string title;
        protected:
            Label() {
                this->title = "";
            }
        public:
            Label(std::string title) {
                this->title = title;
            }

            std::string getTitle();
            virtual std::string encodeLabel();


        friend class StringLabel;
        friend class IntLabel;
        friend class DoubleLabel;
        friend class LabelBuilder;
    };
    
    class StringLabel : public Label {
        private:
            std::string value;
        protected:
            StringLabel(std::string& encodedString);
        public:
            StringLabel(std::string title, std::string value);
            std::string& getValue();
            void setValue(std::string value);
            std::string encodeLabel() override;

        friend class LabelEncoder;
    };
    
    class IntLabel : public Label {
        private:
            int32_t value;
        protected:
            IntLabel(std::string& encodedString);
        public:
            IntLabel(std::string title, int32_t value);
            int32_t& getValue();
            void setValue(int32_t value);
            std::string encodeLabel() override;

        friend class LabelEncoder;
    };
    
    class DoubleLabel : public Label {
        private:
            double value;
        protected:
            DoubleLabel(std::string& encodedString);
        public:
            DoubleLabel(std::string title, double value);
            double& getValue();
            void setValue(double value);
            std::string encodeLabel() override;

        friend class LabelEncoder;
    };
}

#endif