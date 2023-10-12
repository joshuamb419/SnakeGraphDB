#ifndef SG_LABEL
#define SG_LABEL

#include <vector>
#include <string>

namespace SnakeGraph {

/*
 * Notes on the intended usages of Label:
 * - Labels are supposed to be fast access data that stays in memory for sorting between nodes
 * - Labels are not meant to be directly manipulated by users, this datatype should stay within the SnakeGraph library
 * - Labels can be compared to eachother
 *
 * Label Types:
 * 0: No data
 * 1: String data
 * 2: 32bit Int
 * 3: Double
 * 4: Bool
 *
 * Label Encoding Standard:
 * <label_type><label_title>:<value>
 */

    class Label {
        private:
            std::string title;
            int type;
        protected:
            Label() {
                this->title = "";
                this->type = 0;
            }
            Label(std::string title, int type) {
                this->title = title;
                this->type = type;
            }
        public:
            Label(std::string title) {
                this->title = title;
                this->type = 0;
            }

            // Returns the title of the Label
            std::string getTitle();

            // Creates a serialized version of the label as a vector of unsigned chars, this can be passed to LabelDecoder's decodeLabel function to generate a label of the correct type
            virtual std::vector<unsigned char> encodeLabel();

            // For dataless Labels compares titles, otherwise the values are compared. Labels of different types are always not equal
            const bool equals(const Label* label);
            const bool equals(const Label& label);

        friend class StringLabel;
        friend class IntLabel;
        friend class DoubleLabel;
        friend class BoolLabel;
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

    class BoolLabel: public Label {
        private:
            bool value;
        protected:
            BoolLabel(std::vector<unsigned char>& encodedString);
        public:
            BoolLabel(std::string title, bool value);
            bool& getValue();
            void setValue(bool value);
            std::vector<unsigned char> encodeLabel() override;

        friend class LabelDecoder;
    };
}

#endif