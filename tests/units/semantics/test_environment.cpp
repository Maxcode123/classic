#include <gtest/gtest.h>

#include "../../../src/semantics/environment.h"

class VariableEnvironmentProxyTest : public testing::Test {
 protected:
  VariableEnvironment env;
  VariableEnvironmentProxy proxy;

  void SetUp() override {
    this->env = new VariableEnvironment_();
    this->proxy = VariableEnvironmentProxy(this->env);
  }
};

TEST_F(VariableEnvironmentProxyTest, TestUpdateInserts) {
  this->proxy.update("myvar", (new ClassicCustomType_("BaseModel"))->upcast());
  ClassicType t = this->env->get("myvar");
  EXPECT_NE(t, nullptr);

  ClassicCustomType ct = t->downcast<ClassicCustomType>();
  EXPECT_NE(ct, nullptr);
  EXPECT_EQ(ct->type_name, "BaseModel");
}

TEST_F(VariableEnvironmentProxyTest, TestUpdateUpdates) {
  this->env->update("myvar", (new ClassicCustomType_("BaseModel"))->upcast());
  this->proxy.update("myvar", (new ClassicCustomType_("NewModel"))->upcast());
  ClassicType t = this->env->get("myvar");
  EXPECT_NE(t, nullptr);

  ClassicCustomType ct = t->downcast<ClassicCustomType>();
  EXPECT_NE(ct, nullptr);
  EXPECT_EQ(ct->type_name, "NewModel");
}

TEST_F(VariableEnvironmentProxyTest, TestContains) {
  this->env->update("myvar", (new ClassicCustomType_("BaseModel"))->upcast());
  EXPECT_TRUE(this->proxy.contains("myvar"));
}

TEST_F(VariableEnvironmentProxyTest, TestNotContains) {
  EXPECT_FALSE(this->proxy.contains("myvar"));
}

TEST_F(VariableEnvironmentProxyTest, TestGet) {
  this->env->update("myvar", (new ClassicCustomType_("BaseModel"))->upcast());
  ClassicType t = this->proxy.get("myvar");
  EXPECT_NE(t, nullptr);

  ClassicCustomType ct = t->downcast<ClassicCustomType>();
  EXPECT_NE(ct, nullptr);
  EXPECT_EQ(ct->type_name, "BaseModel");
}

TEST_F(VariableEnvironmentProxyTest, TestGetNull) {
  EXPECT_EQ(this->proxy.get("myvar"), nullptr);
}

TEST_F(VariableEnvironmentProxyTest, TestClear) {
  this->env->update("myvar", (new ClassicCustomType_("BaseModel"))->upcast());
  this->env->update("myvar2", (new ClassicCustomType_("BaseModel2"))->upcast());

  EXPECT_NE(this->env->size(), 0);
  this->proxy.clear();
  EXPECT_EQ(this->env->size(), 0);
}
