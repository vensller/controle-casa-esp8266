import styled from 'styled-components/native';

export const Container = styled.SafeAreaView`
  flex: 1;
  padding: 10px;
`;

export const Title = styled.Text`
  font-size: 20px;
  color: #fff;
  font-weight: bold;
  align-self: center;
  margin: 30px 0;
`;

export const Text = styled.Text`
  color: #fff;
  font-weight: bold;
  font-size: 16px;
`;

export const RestartButton = styled.TouchableOpacity`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  margin: 10px 0;
  padding: 10px;
  border: 0;
  background: #2d2f5e;
  border-radius: 4px;
`;

export const ReloadButton = styled.TouchableOpacity`  
  padding: 10px;
  border: 0;
  align-self: center;
`;

export const LogsContainer = styled.ScrollView`
  display: flex;
  flex-direction: column;
  padding: 10px;
  border: 1px solid #eee;
  border-radius: 4px;
  width: 100%;
  height: 100%;
`;