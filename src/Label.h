#ifndef SG_LABEL
#define SG_LABEL

#include <vector>
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
 * <label_type><label_title>:<value>
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
            virtual std::vector<unsigned char> encodeLabel();


        friend class StringLabel;
        friend class IntLabel;
        friend class DoubleLabel;
    };
    
    class StringLabel : public Label {
        private:
            std::string value;
        protected:
            StringLabel(std::vector<unsigned char>& encodedString);
        public:
            StringLabel(std::string title, std::string value);
            std::string& getValue();
            void setValue(std::string value);
            std::vector<unsigned char> encodeLabel() override;

        friend class LabelDecoder;
    };
    
    class IntLabel : public Label {
        private:
            int32_t value;
        protected:
            IntLabel(std::vector<unsigned char>& encodedString);
        public:
            IntLabel(std::string title, int32_t value);
            int32_t& getValue();
            void setValue(int32_t value);
            std::vector<unsigned char> encodeLabel() override;

        friend class LabelDecoder;
    };
    
    class DoubleLabel : public Label {
        private:
            double value;
        protected:
            DoubleLabel(std::vector<unsigned char>& encodedString);
        public:
            DoubleLabel(std::string title, double value);
            double& getValue();
            void setValue(double value);
            std::vector<unsigned char> encodeLabel() override;

        friend class LabelDecoder;
    };
}

#endif